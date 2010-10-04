/*
**
**	$Filename:	switch.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Reset CAN nodes.
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
#include <stddef.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "testdef.h"

#define CONFIG_LIGHT

static void config(int fd, int module)
{
	CAN_message msg;

	can_fill_message(&msg, SET_ID, BROADCAST_SID, 0);
	can_add_data(&msg, TEST_SID & 0xff);
	can_add_data(&msg, TEST_SID >> 8);
	uart_send_can_msg(fd, &msg);
	sleep(4);

	can_fill_message(&msg, CFGBGN, TEST_SID, 0);
	uart_send_can_msg(fd, &msg);

#if 0
	can_fill_message(&msg, PING,   TEST_SID, 0);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
	uart_send_can_msg(fd, &msg);
#endif

#ifndef CONFIG_LIGHT
	can_fill_message(&msg, CFGSWN, TEST_SID, TEST_SWITCH1);
	can_add_data(&msg,  0);
	can_add_data(&msg,  1);
	can_add_data(&msg,  8);
	can_add_data(&msg,  9);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, TEST_SID, TEST_SWITCH2);
	can_add_data(&msg,  2);
	can_add_data(&msg,  3);
	can_add_data(&msg, 10);
	can_add_data(&msg, 11);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, TEST_SID, TEST_SWITCH3);
	can_add_data(&msg,  4);
	can_add_data(&msg,  5);
	can_add_data(&msg, 12);
	can_add_data(&msg, 13);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGSWN, TEST_SID, TEST_SWITCH4);
	can_add_data(&msg,  6);
	can_add_data(&msg,  7);
	can_add_data(&msg, 14);
	can_add_data(&msg, 15);
	uart_send_can_msg(fd, &msg);

	can_fill_message(&msg, CFGRAI, TEST_SID,  TEST_RAIL1);
	can_add_data(&msg, 16);
	can_add_data(&msg, 23);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGRAI, TEST_SID,  TEST_RAIL2);
	can_add_data(&msg, 17);
	can_add_data(&msg, 22);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGRAI, TEST_SID,  TEST_RAIL3);
	can_add_data(&msg, 18);
	can_add_data(&msg, 21);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGRAI, TEST_SID,  TEST_RAIL4);
	can_add_data(&msg, 19);
	can_add_data(&msg, 20);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGML3, TEST_SID,  TEST_MAIN_SIGNAL);
	can_add_data(&msg, module * 16 + 23);
	can_add_data(&msg, module * 16 + 22);
	can_add_data(&msg, module * 16 + 21);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGPL3, TEST_SID,  TEST_PRE_SIGNAL1);
	can_add_data(&msg, module * 16 + 19);
	can_add_data(&msg, module * 16 + 18);
	can_add_data(&msg, module * 16 + 17);
	can_add_data(&msg, module * 16 + 16);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGPL3, TEST_SID,  TEST_PRE_SIGNAL2);
	can_add_data(&msg, module * 16 + 7);
	can_add_data(&msg, module * 16 + 6);
	can_add_data(&msg, module * 16 + 5);
	can_add_data(&msg, module * 16 + 4);
	uart_send_can_msg(fd, &msg);	

	can_fill_message(&msg, CFGSL2, TEST_SID,  TEST_RAIL_SIGNAL);
	can_add_data(&msg, module * 16 + 15);
	can_add_data(&msg, module * 16 + 14);
	uart_send_can_msg(fd, &msg);	
#else
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		can_fill_message(&msg, CFGLGT, TEST_SID,  TEST_LIGHT + i);
		can_add_data(&msg,   i);
		can_add_data(&msg, 26 * i + 12);
		can_add_data(&msg,   15);
		uart_send_can_msg(fd, &msg);	
	}
#endif

	can_fill_message(&msg, CFGEND, TEST_SID, 0);
	uart_send_can_msg(fd, &msg);
}

int main(int argc,char *argv[])
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

	config(fd, 1);

	close(fd);
	return EXIT_SUCCESS;
}
