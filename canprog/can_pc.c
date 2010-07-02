/*
**
**	$Filename:	can_pc.c $
**	$Revision: 967 $
**	$Date: 2010-06-06 20:20:38 +0200 (So, 06. Jun 2010) $
**	$Author: smork $
**	$Id: can_pc.c 967 2010-06-06 18:20:38Z smork $
**
**	RS232/CAN receiver module
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

#include "can_pc.h"
#include <stdio.h>
#include <stddef.h>
#include <termios.h>
#include <fcntl.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

//#define BAUD 230400
#define BAUD B115200
//#define BAUD B57600

#include "mrw_dump.h"

int uart_open(char *name)
{
	struct termios oldtio,newtio;	
	int fd = open(name, O_RDWR|O_NOCTTY);
	if (fd >= 0)
	{
		tcgetattr(fd,&oldtio);
		bzero(&newtio, sizeof(newtio));
		newtio.c_cflag = BAUD | CS8 | CLOCAL | CREAD;
		newtio.c_iflag = IGNPAR;
		newtio.c_oflag = 0;
		newtio.c_lflag = 0;
		newtio.c_cc[VTIME]    = 9;
		newtio.c_cc[VMIN]     = 0;
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd,TCSANOW,&newtio);
	}
	return fd;
}

int uart_send_can_msg(int fd, CAN_message *msg)
{
	unsigned char *ptr = (unsigned char *)msg;
	unsigned char  sum = 0;
	size_t         len = offsetof(CAN_message, data[msg->length]);
	int            i, result;

	for (i = 0;i < len; i++)
	{
		sum -= *ptr++;
	}
	dump_mrw_msg(msg, sum, ">");

	result  = write(fd, msg, len++);
	result += write(fd, &sum, 1);
	if (result != len)
	{
		fprintf(stderr, "%ld != %d\n", len, result);
	}
//	usleep(1000);
	return result;
}

void uart_sync(int fd)
{
	uint8_t byte = 0;
	int     i;

	for (i = 0;i < 16;i++)
	{
		write(fd, &byte, 1);
	}
}

int uart_send_can_data(int fd, unsigned short id, unsigned char *buffer, int length)
{
	CAN_message msg;
	int         i;

	msg.length = length;
	msg.status = 0;
	msg.sid    = id;
	msg.eid    = 0;

 	for (i = 0;i < length; i++)
	{
		msg.data[i] = buffer[i];
	}

	return uart_send_can_msg(fd, &msg);
}

int uart_send_ext_can_data(int fd, unsigned long id, unsigned char *buffer, int length)
{
	CAN_message msg;
	int         i;

	msg.length = length;
	msg.status = 1 << FRAME_EXT;
	msg.sid    = id & SID_MASK;
	msg.eid    = id >> 16;

 	for (i = 0;i < length; i++)
	{
		msg.data[i] = buffer[i];
	}

	return uart_send_can_msg(fd, &msg);
}

void uart_init(receive_buffer *buffer)
{
	bzero(buffer, sizeof(receive_buffer));
}

int uart_receive(receive_buffer *buffer, uint8_t b)
{
	int complete = 0;
//	printf("%2d: %02x\n", buffer->index, b);

	buffer->input.buffer[buffer->index++] = b;
	buffer->checksum += b;

	if ((buffer->input.can.length == 0) || (buffer->input.can.length > 8))
	{
		uart_init(buffer);
		complete = -1;
	}
	else if ((buffer->input.can.length + offsetof(CAN_message, data) + 1) == buffer->index)
	{	
		if (buffer->checksum == 0)
		{
			complete = 1;
		}
		else
		{
			uart_init(buffer);
			complete = -1;
		}
	}
	return complete;
}

void can_add_data(CAN_message *msg, uint8_t data)
{
	msg->data[msg->length++] = data;
}

void  can_fill_message(CAN_message *msg, uint8_t cmd, uint16_t id, uint16_t no)
{
	msg->sid    = id;
	msg->eid    = no;
	msg->length = 0;
	msg->status = no != 0 ? (1 << FRAME_EXT) : 0;
	can_add_data(msg, cmd);
}
