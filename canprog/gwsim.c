/*
**
**	$Filename:	gwsim.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Gateway simulator
**
**	Copyright (C) 2011 committers of this modelrailway project. All rights reserved.
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

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "mrw_dump.h"

static receive_buffer buffer;
static CAN_message    msg;

static void fill_msg(
	uint8_t  cmd,
	uint16_t cid,
	uint16_t unit_no,
	uint8_t  code)
{

	msg.sid     = GATEWAY_SID;
	msg.eid     = cid;

	msg.length  = 0;
	msg.status  = 1 << FRAME_EXT;
	msg.data[msg.length++] = cmd | MSG_RESULT;
	msg.data[msg.length++] = code;
	msg.data[msg.length++] = cid & 0xff;
	msg.data[msg.length++] = cid >> 8;
	msg.data[msg.length++] = unit_no & 0xff ;
	msg.data[msg.length++] = unit_no >> 8;
}

static int ctrlcnt = 0;

static void send_result(
	int      fd,
	uint8_t  cmd,
	uint16_t cid,
	uint16_t unit_no,
	uint8_t  code)
{
	if ((cid == BROADCAST_SID) && (ctrlcnt > 0))
	{
		for (cid = 1; cid <= ctrlcnt; cid++)
		{
			fill_msg(cmd, cid, 0, code);
			uart_send_can_msg(fd, &msg);
		}
	}
	else
	{
		fill_msg(cmd, cid, unit_no, code);
		uart_send_can_msg(fd, &msg);
	}
}

static void send_info(
	int      fd,
	uint8_t  cmd,
	uint16_t cid,
	uint16_t unit_no,
	uint8_t  code,
	uint8_t  info)
{
	if ((cid == BROADCAST_SID) && (ctrlcnt > 0))
	{
		for (cid = 1; cid <= ctrlcnt; cid++)
		{
			fill_msg(cmd, cid, 0, code);
			msg.data[msg.length++] = info;
			uart_send_can_msg(fd, &msg);
		}
	}
	else
	{
		fill_msg(cmd, cid, unit_no, code);
		msg.data[msg.length++] = info;
		uart_send_can_msg(fd, &msg);
	}
}

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty [ctrlcnt]\n", argv[0]);
		return EXIT_SUCCESS;
	}

	if (argc > 2)
	{
		int cnt = 0;
		
		if (sscanf(argv[2], "%d", &cnt) == 1)
		{
			ctrlcnt = cnt;
			printf("Simulating %d controllers.\n", ctrlcnt);
		}
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	while(1)
	{
		unsigned char c[16];
		size_t read_bytes;

		read_bytes = read(fd, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;
			
			for (i = 0; i < read_bytes;i++)
			{
				if (uart_receive(&buffer, c[i]) > 0)
				{
					dump_mrw_msg(&buffer.input.can, buffer.checksum, "<");
					switch(buffer.input.can.data[0])
					{
					default:
						send_result(fd, buffer.input.can.data[0],
							buffer.input.can.sid, buffer.input.can.eid,
							MSG_OK); 
						break;

					case RESET:
						send_info(fd, buffer.input.can.data[0],
							buffer.input.can.sid, buffer.input.can.eid,
							MSG_BOOTED, 0); 
						break;
						
					case GETRBS:
					case GETDIR:
						send_info(fd, buffer.input.can.data[0],
							buffer.input.can.sid, buffer.input.can.eid,
							MSG_OK, 0); 
						break;
					}
					uart_init(&buffer);
				}
			}
		}
		else
		{
			uart_init(&buffer);
		}
	}
	close(fd);
	return EXIT_SUCCESS;
}
