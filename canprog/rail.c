/*
**
**	$Filename:	rail.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Testing rail turning.
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
#include "testdef.h"

static void switch_it(int fd, int unit_no, int cmd)
{
	CAN_message msg;
	
	msg.sid    = TEST_SID;
	msg.eid    = unit_no;
	msg.length = 1;
	msg.status = 1 << FRAME_EXT;
	msg.data[0] = cmd;
	uart_send_can_msg(fd, &msg);
}

static void switch_group(int fd, int i)
{
	switch_it (fd, TEST_RAIL1, i & 1 ? SETRON : SETROF);
	switch_it (fd, TEST_RAIL2, i & 2 ? SETRON : SETROF);
	switch_it (fd, TEST_RAIL3, i & 4 ? SETRON : SETROF);
	switch_it (fd, TEST_RAIL4, i & 8 ? SETRON : SETROF);
}

int main(int argc,char *argv[])
{
	int i;

	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty [relais]\n", argv[0]);
		return EXIT_SUCCESS;
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	uart_sync(fd);

	if (argc > 2)
	{
		if (sscanf(argv[2], "%d", &i) == 1)
		{
			switch_group(fd, i);
		}
	}
	else
	{
		for (i = 1;1;i++)
		{
			switch_group(fd, i);
			if ((i & 15) == 0)
			{
				printf("=======================\n");
				sleep(2);
			}
			sleep(1);
		}
	}
	close(fd);

	return EXIT_SUCCESS;
}
