/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Timer handling
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
