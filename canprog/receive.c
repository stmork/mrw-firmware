/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	RS232 receiver.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "mrw_dump.h"

static receive_buffer buffer;

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

	while (1)
	{
		unsigned char c[16];
		size_t read_bytes;

		read_bytes = read(fd, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;

			for (i = 0; i < read_bytes; i++)
			{
				int result = uart_receive(&buffer, c[i]);
				switch (result)
				{
				case 1:
					dump_mrw_msg(&buffer.input.can, buffer.checksum, "<");
				case -1:
					uart_init(&buffer);
					break;
				}
			}
		}
		else
		{
			uart_init(&buffer);
		}
	}
	close(fd);
	return EXIT_SUCCESS;
}
