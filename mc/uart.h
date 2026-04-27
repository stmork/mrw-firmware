/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	UART handler
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
