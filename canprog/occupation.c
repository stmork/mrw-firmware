/*
**
**	$Filename:	occupation.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Testing rail occupation.
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

static void test_occupation(int fd, int unit_no)
{
	CAN_message msg;

	msg.sid    = TEST_SID;
	msg.eid    = unit_no;
	msg.length = 1;
	msg.status = 1 << FRAME_EXT;
	msg.data[0] = GETRBS;
	uart_send_can_msg(fd, &msg);
}

int main(int argc, char * argv[])
{
	int i;

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

#if 0
	for (i = 0; i < 4; i++)
#else
	for (i = 0; 1; i++)
#endif
	{
		test_occupation(fd, TEST_RAIL1);
		test_occupation(fd, TEST_RAIL2);
		test_occupation(fd, TEST_RAIL3);
		test_occupation(fd, TEST_RAIL4);
		sleep(1);
	}

	close(fd);
	return EXIT_SUCCESS;
}
