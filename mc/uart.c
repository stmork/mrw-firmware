/*
**
**	$Filename:	uart.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	UART handler
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

#ifndef F_CPU
#define F_CPU 14745600UL
#endif

#include "uart.h"
#include "bit.h"

#define BAUD 115200UL

#define UBRR_VAL      (F_CPU       / (BAUD << 4) - 1)
#define REAL          (F_CPU * 100 / (BAUD << 4))
#define BAUD_ERROR    (((UBRR_VAL + 2) * 100 - REAL) % 100)

#if (BAUD_ERROR > 1)
#error "Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!"
#endif

#define UART_TX_WAIT loop_until_bit_is_set(UCSRA, UDRE)
#define UART_RX_WAIT loop_until_bit_is_set(UCSRA, RXC)

/**
 * Hier wird die UART Schnittstelle auf synchrone Verbindung
 * bei 115200 Baud und 8N1 Bitverteilung programmiert.
 */
void uart_init(void)
{
	uint8_t i;

	// set baud rate
	UBRRH = UBRR_VAL >> 8;
	UBRRL = UBRR_VAL;

	// enable rx/tx and rx interrupt
	UCSRB = _BV(TXEN)  | _BV(RXEN)  | _BV(RXCIE);

	// 8 data bits, 1 stop bit, no parity (8N1)
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

	// Reset PC receive buffer.
	uart_tx_byte(0xff);
	uart_tx_byte(0xaa);
	uart_tx_byte(0x55);
	for (i = 0; i < 16;i++)
	{
		uart_tx_byte(0);
	}
}

/**
 * Diese Routine versendet ein Byte.
 */
void uart_tx_byte(uint8_t val)
{
	UART_TX_WAIT;
	UDR = val;
}

/**
 * Diese Routine empfängt ein Byte.
 */
uint8_t uart_rx_byte(void)
{
	UART_RX_WAIT;
	return UDR;
}
