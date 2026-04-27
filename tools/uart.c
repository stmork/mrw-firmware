/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	UART frequency error rate calculator
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
