/*
**
**	$Filename:	serial.h $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: serial.h 931 2010-04-14 08:39:15Z smork $
**
**	Serial shift handler
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

#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>

#include "bit.h"

#define DDR_SERIAL  DDRB
#define PORT_SERIAL PORTB

#define P_DATA     0   // SER    Wanne 7
#define P_CLK      1   // SCK    Wanne 5
#define P_SET      3   // RCK    Wanne 3

#define SER_CLK    _BV(P_CLK)
#define SER_DATA   _BV(P_DATA)
#define SER_SET    _BV(P_SET)

#define SET_SER_BIT(byte,bit) { serial_buffer[byte] |= set_bits[bit]; }
#define CLR_SER_BIT(byte,bit) { serial_buffer[byte] &= clr_bits[bit]; }

#define MAX_SERIAL_BUFFER 8

extern uint8_t serial_buffer[MAX_SERIAL_BUFFER];

extern void serial_init(void);
extern void serial_put_buffer(uint8_t *buffer, uint8_t len);
extern void clear_serial_buffer(void);
extern void send_serial_buffer(void);

#endif
