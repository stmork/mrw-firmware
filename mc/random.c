/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Compute random number
**
*/

#include <avr/io.h>

#include "config.h"
#include "random.h"

void random_preinit(void)
{
	/* Kein Prescaling */
	TCCR0 = _BV(CS00);
}

/**
 * Diese Methode führt einen Seed durch und initialisiert den
 * Timer entsprechend.
 */
void random_postinit(void)
{	
	uint8_t *ptr = (uint8_t *)(RAMEND & 0xff);
	uint8_t  l = 0;

	/*
	 * Initialisierung mit Zufallszahlen. Das RAM scheint hierfür
	 * am Besten geeignet zu sein.
	 */
	uint8_t rnd = config.id ^ TCNT2;
	for (uint8_t h = RAMEND >> 8; h > 0; h--)
	{
		do
		{
			rnd ^= *ptr++;
			
			l++;
		}
		while(l != 0);
	}
	TCNT0 ^= rnd;
}

/**
 * Die Zufallszahl wird aus dem laufenden Timer einfach
 * ausgelesen.
 */
uint8_t random_timer(void)
{
	return TCNT0;
}
