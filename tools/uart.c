/*
**
**	$Filename:	uart.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	UART frequency error rate calculator
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

#define BAUD 115200.0

int main(int argc, char * argv[])
{
	long ubbr;

	for (ubbr = 1; ubbr < 16; ubbr++)
	{
		printf("UBBR: %3ld = %1.5f MHz\n", ubbr,
			(ubbr + 1) * BAUD * 16.0);
	}
	return EXIT_SUCCESS;
}
