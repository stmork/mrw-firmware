/*
**
**	$Filename:	bit.c $
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

void timer0_init(void)
{
	// prescaler 1024
	TCCR0  = _BV(CS00) | _BV(CS02);

	// interrupt enable
	TIMSK |= _BV(TOIE0);
}

void timer2_init(void)
{
	// prescaler 1024
	TCCR2  = _BV(CS00) | _BV(CS01) | _BV(CS02);

	// interrupt enable
	TIMSK |= _BV(TOIE2);
}
