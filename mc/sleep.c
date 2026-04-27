/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Sleep mode handling
**
*/

#include "sleep.h"
#include "bit.h"

#include <avr/sleep.h>

/**
 * Diese Methode führt einfach einen Sleep aus. Der
 * Zustand wird durch einen Interrupt abgebrochen.
 */
void sleep(void)
{
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
}
