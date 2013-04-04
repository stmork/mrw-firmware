/*
**
**	$Filename:	bit.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Bit handling
**
**	Copyright (C) 2013 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**	which accompanies this distribution.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#ifndef BIT_H
#define BIT_H

#include <stdint.h>

#ifdef AVR
#include <avr/pgmspace.h>
#endif

#define SET_PORT_BIT(port,pin)       ((port) |=  _BV(pin))
#define CLR_PORT_BIT(port,pin)       ((port) &= ~_BV(pin))
#define TST_PORT_BIT(port,pin)       bit_is_set(port,pin)
#define TST_PORT_BIT_NEG(port,pin) (!bit_is_set(port,pin))

#define CONNECTION_BITS_MASK 0x07
#define CONNECTION_PORT_MASK 0xf8

#define CONNECTION_PORT(con) ((uint8_t)((con) & CONNECTION_PORT_MASK))
#define CONNECTION_BITS(con) ((uint8_t)((con) & CONNECTION_BITS_MASK))

#define PIN_RISING_EDGE  -1
#define PIN_NO_CHANGE     0
#define PIN_FALLING_EDGE  1

#define DEPRELL_COUNT     5

typedef struct
{
	uint8_t bit;
} mrw_connection;

typedef struct
{
	mrw_connection connection;
	uint8_t        counter;
	uint8_t        last_state:1;
	uint8_t        state:1;
} mrw_input;

#ifdef AVR
extern const uint8_t set_bits[8] PROGMEM;
extern const uint8_t clr_bits[8] PROGMEM;
#endif

extern void    set_pin(mrw_connection *con);
extern void    clr_pin(mrw_connection *con);
extern void    out_pin(mrw_connection *con);
extern void    in_pin (mrw_connection *con, uint8_t pullup);
extern uint8_t tst_pin(mrw_connection *con);

extern void    config_connection(mrw_connection *con, uint8_t con_no);
extern void    init_input(mrw_input *input, uint8_t pullup);
extern int8_t  test_state_change(mrw_input *input);

#endif
