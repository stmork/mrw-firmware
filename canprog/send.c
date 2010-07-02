/*
**
**	$Filename:	ping.c $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: send.c 931 2010-04-14 08:39:15Z smork $
**
**	Ping a gateway.
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
#include <string.h>
#include <unistd.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"

static int flood =   0;
static int max   = 500;

static int read_integer(char *buffer)
{
	int value = 0;

	if (strcmp(buffer, "0x") != 0)
	{
		sscanf(&buffer[2], "%x", &value);
	}
	else 
	{
		sscanf(buffer, "%d", &value);
	}
	return value;
}

int main(int argc,char *argv[])
{
	int           id, i;
	unsigned char buffer[8];

	if ((argc <= 3) || (argc > 11))
	{
		printf("USAGE:\n");
		printf("%s tty id [hexbyte1 hexbyte2 ... hexbyte8]\n", argv[0]);
		return EXIT_SUCCESS;
	}

	// parse id
	id = read_integer(argv[2]);

	// parse and copy data bytes to send into buffer
	for (i = 3; i < argc; i++)
	{
		buffer[i-3] = read_integer(argv[i]);
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	uart_sync(fd);

	i = 0;
	do
	{
		uart_send_can_data(fd, id, buffer, argc-3);
		if (!flood)
		{
			sleep(1);
		}
	}
	while(++i < max);

	close(fd);
	return EXIT_SUCCESS;
}
