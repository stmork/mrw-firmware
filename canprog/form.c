/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Reset CAN nodes.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "testdef.h"

static void turn_signal(int fd, int pf_signal, int mf_signal)
{
	CAN_message msg;

	msg.sid     = 1;
	msg.length  = 2;
	msg.status  = 1 << FRAME_EXT;
	msg.data[0] = SETSGN;

	msg.eid     = 0xcbd;
	msg.data[1] = pf_signal;
	uart_send_can_msg(fd, &msg);

	msg.eid     = 0xc8c;
	msg.data[1] = mf_signal;
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

	do
	{
		turn_signal(fd, SIGNAL_VR0, SIGNAL_HP0);
		sleep(3);
		turn_signal(fd, SIGNAL_VR1, SIGNAL_HP1);
		sleep(3);
		turn_signal(fd, SIGNAL_VR2, SIGNAL_HP2);
		sleep(3);
		turn_signal(fd, SIGNAL_OFF, SIGNAL_HP0);
		sleep(3);
	}
	while (1);

	close(fd);
	return EXIT_SUCCESS;
}
