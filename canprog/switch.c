/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Testing switch turning.
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
		switch_it (fd, TEST_SWITCH3, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH4, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH1, i & 1 ? SETLFT : SETRGT);
		switch_it (fd, TEST_SWITCH2, i & 1 ? SETLFT : SETRGT);
		sleep(5);
	}

	close(fd);
	return EXIT_SUCCESS;
}
