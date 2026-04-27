/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Gateway rate calculator
**
*/

#include <stdio.h>
#include <stdlib.h>

#define F_CPU   14745600L
#define F_BAUD    115200L
#define F_CAN     125000L
#define F_SPI  (F_CPU / 2)

int main(int argc, char * argv[])
{
	int i;

	printf("Frame rates:\n");
	printf("bytes: RS232   SPI CANsf CANef\n");
	for (i = 0; i <= 8; i++)
	{
		int Fr_baud   = F_BAUD / (i * 10 + 60);
		int Fr_spi    = F_SPI  / (i *  8 + 80);
		int Fr_can_sf = F_CAN  / (i *  8 + 44);
		int Fr_can_ef = F_CAN  / (i *  8 + 64);

		printf("%5d: %5d %5d %5d %5d\n", i,
			Fr_baud,
			Fr_spi,
			Fr_can_sf,
			Fr_can_ef);
	}
	return EXIT_SUCCESS;
}
