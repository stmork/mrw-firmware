/*
**
**	$Filename:	random.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Compute random number
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

#include "random.h"

#include <avr/io.h>

void random_init(void)
{
	uint8_t *ptr = (uint8_t *)0x60;

	/* Kein Prescaling */
	TCCR0 = _BV(CS00);
	
	/*
	 * Initialisierung mit Zufallszahlen. Das RAM scheint hierfür
	 * am Besten geeignet zu sein.
	 */
	for (uint16_t i = 0;i < 2048;i++)
	{
		TCNT0 += *ptr++;
	}
}

uint8_t random_timer(void)
{
	return TCNT0 >> 1;
}
