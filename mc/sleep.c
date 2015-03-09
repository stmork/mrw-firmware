/*
**
**	$Filename:	sleep.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Sleep mode handling
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
