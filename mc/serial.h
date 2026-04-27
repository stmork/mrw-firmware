/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Serial shift handler
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

#define SET_SER_BIT(byte,bit) { serial_buffer[byte] |= pgm_read_byte(&set_bits[bit]); }
#define CLR_SER_BIT(byte,bit) { serial_buffer[byte] &= pgm_read_byte(&clr_bits[bit]); }

#define MAX_SERIAL_BUFFER 8

extern uint8_t serial_buffer[MAX_SERIAL_BUFFER];

extern void serial_limit(uint8_t byte);
extern void serial_init(void);
extern void serial_put_buffer(uint8_t *buffer, uint8_t len);
extern void clear_serial_buffer(void);
extern void send_serial_buffer(void);

#endif
