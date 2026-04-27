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

#include "can_pc.h"

int main(int argc, char * argv[])
{
	unsigned char zero = 0;

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

	printf("Testing busy indicator of CAN-gateway.\n");
	printf("Please press CTRL+C to exit!\n");

	while (1)
	{
		if (write(fd, &zero, sizeof(zero)) <= 0)
		{
			perror(argv[1]);
		}
	}

	return EXIT_SUCCESS;
}
