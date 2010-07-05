/*
**
**	$Filename:	spi.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Modellrailway micro controller
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

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#define DDR_SPI     DDRB
#define PORT_SPI    PORTB
#define P_MISO      6
#define P_MOSI      5
#define P_SCK       7

// SPI commands
#define SPI_RESET		0xC0
#define	SPI_READ		0x03
#define	SPI_READ_RX		0x90
#define	SPI_WRITE		0x02
#define	SPI_LOAD_TX		0x40
#define	SPI_RTS			0x80
#define SPI_READ_STATUS	0xA0
#define	SPI_RX_STATUS	0xB0
#define	SPI_BIT_MODIFY	0x05

#define SPI_R002     (0)
#define SPI_R004     (0)
#define SPI_R008                                  (1 << SPR0)
#define SPI_R016                                  (1 << SPR0)
#define SPI_R032                    (1 << SPR1)
#define SPI_R064                   ((1 << SPR1) | (1 << SPR0))
#define SPI_R128                   ((1 << SPR1) | (1 << SPR0))

#define SPI_X2002    (1 << SPI2X)
#define SPI_X2004    (0)
#define SPI_X2008    (1 << SPI2X)
#define SPI_X2016    (0)
#define SPI_X2032    (1 << SPI2X)
#define SPI_X2064    (1 << SPI2X)
#define SPI_X2128    (0)

extern void    spi_init(void);
extern uint8_t spi_getc(void);
extern void    spi_putc(uint8_t data);

#endif
