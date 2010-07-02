/*
**
**	$Filename:	switch.c $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: switch.c 931 2010-04-14 08:39:15Z smork $
**
**	Testing switch turning.
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

int main(int argc,char *argv[])
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
	for (i = 0;i < 4;i++)
#else
	for (i = 0;1;i++)
#endif
	{
		switch_it (fd, TEST_SWITCH3, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH4, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH1, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH2, i & 1 ? SETLFT : SETRGT);
		sleep(5);
	}

	close(fd);
	return EXIT_SUCCESS;
}
