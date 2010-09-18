/*
**
**	$Filename:	pwm.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	PWM soft handler
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

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#define PORT_LIGHT   PORTA
#define DDR_LIGHT    DDRA
#define PIN_LIGHT    PINA
#define P_LIGHT_LED0 0
#define P_LIGHT_LED1 1
#define LIGHT_LED0   _BV(P_LIGHT_LED0)   // red
#define LIGHT_LED1   _BV(P_LIGHT_LED1)  // green

#define PWM_TABLE_SIZE 32

extern void set_dimm(uint8_t idx, uint8_t value);
extern void handle_pwm(void);

#endif
