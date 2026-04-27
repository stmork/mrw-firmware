/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Query buffer state.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"

static int qryerr(int fd)
{
	unsigned char  buffer[8];

	buffer[0] = QRYERR;
	return uart_send_can_data(fd, BROADCAST_SID, buffer, 1);
}

int main(int argc, char * argv[])
{
	int result;

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

	result = qryerr(fd);
	close(fd);

	return result > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
