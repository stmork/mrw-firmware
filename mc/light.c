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
#define F_CPU 16000000L
#endif

#include <stdlib.h>
#include <avr/interrupt.h>

#include "pwm.h"
#include "sleep.h"
#include "timer.h"

static uint16_t counter = 0;

/*
 * Hier werden die beiden LED Port Pins auf Ausgang geschaltet.
 */
static void port_init(void)
{
	/* LED Pins auf Output setzen und Anzeige löschen. */
	DDR_LIGHT  |=  0xff;
	PORT_LIGHT  =  0;
}

/*
 * Hier wird das Soft-PWM durchgeführt.
 */
ISR(TIMER1_COMPA_vect)
{
	handle_pwm();
}

/*
 * Hier wird die Helligkeit eines jeden Soft-PWM-Kanals einzeln eingestellt.
 */
ISR(TIMER2_OVF_vect)
{
	set_dimm(0, counter << 2);
	set_dimm(1, counter << 1);
	set_dimm(2, counter);
	set_dimm(3, counter >> 1);
	set_dimm(4, counter >> 2);
	set_dimm(5, counter >> 3);
	set_dimm(6, counter >> 4);
	set_dimm(7, counter >> 5);

	counter++;
}

static void init_without_timer()
{
	/* Initialisierung, falls Timer nicht verwendet wird. */
	set_dimm(7, 255);
	set_dimm(6, 224);
	set_dimm(5, 192);
	set_dimm(4, 160);
	set_dimm(3, 128);
	set_dimm(2,  96);
	set_dimm(1,  64);
	set_dimm(0,  32);
}

int main(void)
{
	port_init();
	timer1_init(F_CPU / (50 * PWM_TABLE_SIZE));
	timer2_init();

	sei();

	do
	{
		/*
		 * Ein bißchen was für die Umwelt tun. Es fängt halt
		 * halt schon im Kleinen an ;-)
		 */
		sleep();
	}
	while(1);
}
