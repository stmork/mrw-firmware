/*
**
**	$Filename:	timer.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Timer handling
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

#ifndef TIMER_H
#define TIMER_H

#define TIMER0_SHIFT    18
#define TIMER2_SHIFT    18

#define TIMER0_PRESCALER (1 << (TIMER0_SHIFT - 8))
#define TIMER2_PRESCALER (1 << (TIMER2_SHIFT - 8))

#include <inttypes.h>

extern void timer0_init(void);
extern void timer1_init(uint16_t clock);
extern void timer2_init(void);

#ifdef F_CPU

#define TIMER0_DELAY(s) ((F_CPU * (s)) >> TIMER2_SHIFT)
#define TIMER2_DELAY(s) ((F_CPU * (s)) >> TIMER2_SHIFT)

#endif

#endif
