/*
**
**	$Filename:	busy.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
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

int main(int argc,char *argv[])
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

	while(1)
	{
		write(fd, &zero, sizeof(zero));
	}
	
	return EXIT_SUCCESS;
}
