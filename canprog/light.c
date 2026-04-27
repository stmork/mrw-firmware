/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Testing rail turning.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "testdef.h"

static void sensor(int fd, int lightness)
{
	CAN_message msg;

	msg.sid    = BROADCAST_SID;
	msg.eid    = 0;
	msg.length = 3;
	msg.status = 0;
	msg.data[0] = SENSOR;
	msg.data[1] = SENSOR_LIGHT;
	msg.data[2] = lightness;
	uart_send_can_msg(fd, &msg);
}

int main(int argc, char * argv[])
{
	int lightness;

	if (argc <= 2)
	{
		printf("USAGE:\n");
		printf("%s tty lightness\n", argv[0]);
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
		if (sscanf(argv[2], "%d", &lightness) == 1)
		{
			sensor(fd, lightness);
		}
	}
	close(fd);

	return EXIT_SUCCESS;
}
