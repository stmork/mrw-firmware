/*
**
**	$Filename:	serial.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
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

#include "serial.h"
#include "tool.h"

#define DATA0 { PORT_SERIAL &= (~SER_DATA); }
#define DATA1 { PORT_SERIAL |=   SER_DATA; }

#define CLK0  { PORT_SERIAL &= (~SER_CLK); }
#define CLK1  { PORT_SERIAL |=   SER_CLK; }

#define SET0  { PORT_SERIAL &= (~SER_SET); }
#define SET1  { PORT_SERIAL |=   SER_SET; }

       uint8_t serial_buffer[MAX_SERIAL_BUFFER];
static uint8_t start_idx = sizeof(serial_buffer);

void serial_init(void)
{
	DDR_SERIAL |= SER_CLK | SER_DATA | SER_SET;
	clear_serial_buffer();
	send_serial_buffer();
}

static void serial_putc(uint8_t val)
{
	for (uint8_t bit = 1; bit != 0; bit = bit << 1)
	{
		
		if (val & bit)
		{
			DATA1;
		}
		
		CLK1;
		CLK0;
		DATA0;
	}
}

void serial_put_buffer(uint8_t *buffer, uint8_t len)
{
	DATA0;
	while (len > 0)
	{
		serial_putc(*buffer++);
		len--;
	}
	SET1;
	NOP;
	SET0;
}

void clear_serial_buffer(void)
{
	uint8_t *buffer = serial_buffer;

	DATA0;
	for (uint8_t i = sizeof(serial_buffer); i > 0; i--)
	{
		*buffer++ = 0;
		for (uint8_t bit = 8; bit > 0; bit--)
		{
			CLK1;
			CLK0;
		}
	}
	SET1;
	NOP;
	SET0;
}

void serial_limit(uint8_t byte)
{
	if (byte < start_idx)
	{
		start_idx = byte;
	}
}

void send_serial_buffer(void)
{
	serial_put_buffer(&serial_buffer[start_idx], sizeof(serial_buffer) - start_idx);
}
