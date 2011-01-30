/*
**
**	$Filename:	receive.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	RS232 receiver.
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
#include "mrw_dump.h"

static receive_buffer buffer;

int main(int argc, char *argv[])
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

	while(1)
	{
		unsigned char c[16];
		size_t read_bytes;

		read_bytes = read(fd, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;
			
			for (i = 0; i < read_bytes;i++)
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
