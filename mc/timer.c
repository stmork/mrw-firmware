/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway micro controller
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

/**
 * Diese Methode initialisiert den Timer 1 für die
 * PWM-Ansteuerung. Die Taktrate wurde vorher schon
 * für die benötigte Frequenz berechnet.
 */
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

/**
 * Diese Methode initialisiert den Timer 2 für
 * langlaufende Schaltbefehle wie z.B. Weichen-
 * Schaltungen.
 */
void timer2_init(void)
{
	/* Prescaler 1024 */
	TCCR2  = _BV(CS20) | _BV(CS21) | _BV(CS22);

	/* Interrupt einschalten */
	TIMSK |= _BV(TOIE2);
}
