/*
**
**	$Filename:	timer.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Modellrailway micro controller
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

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void timer0_init(void)
{
	/* Prescaler 1024 */
	TCCR0  = _BV(CS00) | _BV(CS02);

	/* Interrupt einschalten */
	TIMSK |= _BV(TOIE0);
}

void timer1_init(uint16_t clock)
{
	uint8_t sreg = SREG;

	cli();

	/* Takte zwischen Interrupts */
	OCR1A  = clock;
	
	/* Kein OC1x output */
	TCCR1A = 0;

	/* CTC Timer bei Gleichheit zurücksetzen, kein Prescaler */
	TCCR1B = _BV(WGM12) | _BV(CS10);

	/* Interrupt einschalten */
	TIMSK |= _BV(OCIE1A);

	SREG = sreg;
}

void timer2_init(void)
{
	/* Prescaler 1024 */
	TCCR2  = _BV(CS20) | _BV(CS21) | _BV(CS22);

	/* Interrupt einschalten */
	TIMSK |= _BV(TOIE2);
}
