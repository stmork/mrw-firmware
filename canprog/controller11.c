/*
**
**	$Filename:	controller11.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Reset CAN nodes.
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
#include <stddef.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"

#define CONTROLLER_SID 11

static uint8_t values[11] =
{
	0x18, 0x12, 0x1a, 0x1e, 0x2a, 0x20, 0x16, 0x24, 0x1c, 0x10, 0x28
};

static void config_ports(int fd)
{
	CAN_message msg;
	uint8_t i;

	can_fill_message(&msg, SET_ID, BROADCAST_SID, 0);
	can_add_data(&msg, CONTROLLER_SID & 0xff);
	can_add_data(&msg, CONTROLLER_SID >> 8);
	uart_send_can_msg(fd, &msg);
	sleep(4);

	can_fill_message(&msg, CFGBGN, CONTROLLER_SID, 0);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, CONTROLLER_SID, 0x0835);
	can_add_data(&msg,  0);
	can_add_data(&msg,  1);
	can_add_data(&msg,  8);
	can_add_data(&msg,  9);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, CONTROLLER_SID, 0x0836);
	can_add_data(&msg,  2);
	can_add_data(&msg,  3);
	can_add_data(&msg, 10);
	can_add_data(&msg, 11);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, CONTROLLER_SID, 0x0837);
	can_add_data(&msg,  4);
	can_add_data(&msg,  5);
	can_add_data(&msg, 12);
	can_add_data(&msg, 13);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGPF2, CONTROLLER_SID, 0x08a7);
	can_add_data(&msg,  7);
	can_add_data(&msg,  7);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGRAI, CONTROLLER_SID, 0x00fc);
	can_add_data(&msg, 16);
	can_add_data(&msg, 23);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGRAI, CONTROLLER_SID, 0x0227);
	can_add_data(&msg, 17);
	can_add_data(&msg, 22);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGRAI, CONTROLLER_SID, 0x021c);
	can_add_data(&msg, 18);
	can_add_data(&msg, 21);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGRAI, CONTROLLER_SID, 0x01c3);
	can_add_data(&msg, 19);
	can_add_data(&msg, 20);
	uart_send_can_msg(fd, &msg);

	for (i = 0; i < 11; i++)
	{
		can_fill_message(&msg, CFGLGT, CONTROLLER_SID,  0x100e + i);
		can_add_data(&msg,   48 + i);    // Anschlusspin
		can_add_data(&msg,   values[i]);    // Schwellwert
		uart_send_can_msg(fd, &msg);
	}

	can_fill_message(&msg, CFGEND, CONTROLLER_SID, 0);
	uart_send_can_msg(fd, &msg);
}

int main(int argc, char * argv[])
{
	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty\n", argv[0]);
		return EXIT_SUCCESS;
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	uart_sync(fd);

	config_ports(fd);

	close(fd);
	return EXIT_SUCCESS;
}
