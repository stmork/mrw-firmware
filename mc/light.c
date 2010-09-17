/*
**
**	$Filename:	light.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Light dimming test firmware
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

#ifndef F_CPU
#define F_CPU 14745600L
#endif

#include <stdlib.h>
#include "bit.h"
#include "pwm.h"
#include "sleep.h"
#include "timer.h"

/*
 * Hier werden die beiden LED Port Pins auf Ausgang geschaltet.
 */
static void port_init(void)
{
	/* LED Pins auf Output setzen und Anzeige löschen. */
	DDR_LIGHT  |=    LIGHT_LED0 | LIGHT_LED1;
	PORT_LIGHT &= (~(LIGHT_LED0 | LIGHT_LED1));
}

ISR(TIMER2_OVF_vect)
{
	handle_pwm();
}

int main(void)
{
	uint16_t counter = 0;

	port_init();
//	timer2_init();
	TCCR2  = _BV(CS01);
	TIMSK |= _BV(TOIE2);

	sei();

	do
	{
		sleep();
		sei();

		set_dimm(6, counter >> 6);
		set_dimm(7, counter >> 4);
		
		counter++;
	}
	while(1);
}
