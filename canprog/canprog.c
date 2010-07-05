/*
**
**	$Filename:	canprog.c $
**	$Revision: 967 $
**	$Date: 2010-06-06 20:20:38 +0200 (So, 06. Jun 2010) $
**	$Author: smork $
**	$Id: canprog.c 967 2010-06-06 18:20:38Z smork $
**
**	ATmega CAN bootloader progger
**
**	Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**	which accompanies this distribution.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "can_pc.h"
#include "mrw.h"

#define SPM_PAGESIZE 128

// delay after reset (1500 ms)
#define DELAY_RESET 1500000

// delay after flash request (10 ms)
#define DELAY_FLASH_REQUEST 10000

// delay for flashing a page (60 ms)
#define DELAY_FLASH_PAGE 60000

#define GET_WORD(ptr) ((((unsigned short)(ptr)[1]) << 8) | ((ptr)[0]))

#define   DEFAULT_HARDWARE         1

// ATmega32 signature
#define   SIGNATURE_BYTE_1      0x1E
#define   SIGNATURE_BYTE_2      0x95
#define   SIGNATURE_BYTE_3      0x02 

static int fd;
static int use_reset = 1;

static unsigned char * read_hex(const char *filename, size_t *size)
{
	FILE          *file = fopen(filename, "r");
	char           line[128];
	unsigned char *buffer = NULL;
	int            i, idx;
	
	if (file != NULL)
	{
		unsigned int count, address, type;

		while (fgets(line, sizeof(line), file) == line)
		{
			if (sscanf(line, ":%02x%04x%02x", &count, &address, &type) == 3)
			{
				unsigned char *bytes = NULL;
				int checksum = type + count + (address >> 8) + (address & 0xff);

				switch(type)
				{
				case 0: //data
					*size = address + count;
					buffer = (unsigned char *)realloc(buffer, *size + 1);
					bytes = &buffer[address];
					for (i = 0, idx = 9;i <= count;i++)
					{
						int byte = 0;
						sscanf(&line[idx],"%02x", &byte);
						bytes[i] = byte & 0xff;
						checksum += byte;
						idx += 2;
					}
					checksum &= 0xff;
					if (checksum != 0)
					{
						fprintf(stderr, "Checksum error at address 0x%04x: %02x != %02x in file\n",
							address, checksum, bytes[count]);
					}
					break;

				case 1: // EOF
					break;
				}
#if 0
				printf("%2u %04x %d - %4x\n", count, address, type, checksum);
				if (bytes != NULL)
				{
					printf(" %02x", bytes[count]);
				}
				printf("\n");
#endif
			}
		}
		fclose(file);
	}
	return buffer;
}

static void ping(int fd)
{
	unsigned char  buffer[8];
	
	buffer[0] = PING;
	uart_send_can_data(fd, BROADCAST_SID, buffer, 1);
}

static void send_can_data(unsigned short id, unsigned char cmd, unsigned char *buffer, int length)
{
	buffer[0] = cmd;
	uart_send_can_data(fd, id, buffer, length);
}

static void send_can(unsigned short id, unsigned char cmd)
{
	unsigned char buffer[8];
	send_can_data(id, cmd, buffer, 1);
}

static void flash(unsigned char *buffer, int size, int hid)
{
	unsigned int   address = 0;
	unsigned char  checksum = 0;
	unsigned char  msg[8];
	int            i;

	if (use_reset)
	{
		send_can(BROADCAST_SID, RESET);
		usleep(DELAY_RESET);
	}
	
	msg[1] = hid;
	msg[2] = SIGNATURE_BYTE_1;
	msg[3] = SIGNATURE_BYTE_2;
	msg[4] = SIGNATURE_BYTE_3;
	for (i = 0;i < 5;i++)
	{
		send_can_data(BROADCAST_SID, FLASH_REQ, msg, 5);
		usleep(DELAY_FLASH_REQUEST);
	}
	ping(fd);

	while (size >= SPM_PAGESIZE)
	{
		int loop;
		for (loop = 0; loop < SPM_PAGESIZE; loop += 4)
		{
			msg[1] = address & 0xff;
			msg[2] = address >>  8;
			msg[3] = address >> 16;
			msg[4] = buffer[address++];
			msg[5] = buffer[address++];
			msg[6] = buffer[address++];
			msg[7] = buffer[address++];

			checksum += msg[4];
			checksum += msg[5];
			checksum += msg[6];
			checksum += msg[7];
			send_can_data(BROADCAST_SID, FLASH_DATA, msg, 8);
		}
		size -= SPM_PAGESIZE;
		printf("-\n");
		usleep(DELAY_FLASH_PAGE);
	}
	
	while (size >= 0)
	{
		unsigned char msg[8];
		msg[1] = address & 0xff;
		msg[2] = address >> 8;
		msg[3] = address >> 16;
		msg[4] = buffer[address++];
		msg[5] = buffer[address++];

		checksum += msg[4];
		checksum += msg[5];
		send_can_data(BROADCAST_SID, FLASH_DATA, msg, 6);

		size -= 2;
	}
	printf("-\n");
	usleep(DELAY_FLASH_PAGE);

	msg[1] = address & 0xff;
	msg[2] = address >> 8;
	msg[3] = 0;
	msg[4] = checksum;
	send_can_data(BROADCAST_SID, FLASH_CHECK, msg, 5);
}

int main(int argc,char *argv[])
{
	int hid = DEFAULT_HARDWARE;

	if (argc < 3)
	{
		printf("USAGE:\n");
		printf("%s tty hexfile [hid]\n", argv[0]);
		return EXIT_SUCCESS;
	}

	if (argc > 3)
	{
		if (sscanf(argv[3],"%d", &hid) != 1)
		{
			hid = DEFAULT_HARDWARE;
		}
	}

	printf("Hardware version to flash: %d\n", hid);

	fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}
	uart_sync(fd);

	size_t   count = 0;
	unsigned char *buffer = read_hex(argv[2], &count);

	if (buffer != NULL)
	{
		printf("size: %d\n", count);
		flash(buffer, count, hid);
		free(buffer);
	}
	close(fd);
	return EXIT_SUCCESS;
}
