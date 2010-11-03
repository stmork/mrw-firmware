/*
**
**	$Filename:	uart.h $
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

#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UART_ENABLE_TX_IRQ()  SET_PORT_BIT(UCSRB, UDRIE)
#define UART_DISABLE_TX_IRQ() CLR_PORT_BIT(UCSRB, UDRIE)

extern void    uart_init(void);
extern void    uart_tx_byte(uint8_t val);
extern uint8_t uart_rx_byte(void);

#endif
