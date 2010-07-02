/*
**
**	$Filename:	switch.c $
**	$Revision: 967 $
**	$Date: 2010-06-06 20:20:38 +0200 (So, 06. Jun 2010) $
**	$Author: smork $
**	$Id: setid.c 967 2010-06-06 18:20:38Z smork $
**
**	Reset CAN nodes.
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

static void set_id(int fd, int old_id, int new_id)
{
	unsigned char  buffer[8];
	
	buffer[0] = SET_ID;
	buffer[1] = new_id & 0xff;
	buffer[2] = new_id >> 8;
	uart_send_can_data(fd, old_id, buffer, 3);
}

int main(int argc,char *argv[])
{
	int id = TEST_SID;

	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty [id]\n", argv[0]);
		return EXIT_SUCCESS;
	}

	if (argc >= 3)
	{
		if (sscanf(argv[2], "%d", &id) != 1)
		{
			id = TEST_SID;
		}
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	uart_sync(fd);

	set_id(fd, BROADCAST_SID, id);

	close(fd);
	return EXIT_SUCCESS;
}
