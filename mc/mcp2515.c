/*
**
**	$Filename:	mcp2515.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	MCP2515 CAN controller handling
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

#define FILTER_ENABLE

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <avr/interrupt.h>
#include <util/delay.h>

#include "mcp2515.h"
#include "spi.h"
#include "bit.h"
#include "tool.h"

#ifdef FILTER_ENABLE
#define MCP_FILTER_BITS (0)
#else
#define MCP_FILTER_BITS (_BV(RXM1) | _BV(RXM0))
#endif

#define TQ_COUNT  16
#define SYNC_SEG   1
#define SJW        2

#define R_BRP       (((F_MCP / (TQ_COUNT * F_CAN)) >> 1) - 1)
#define R_PS2      (TQ_COUNT * 3 / 10)
#define R_PS1      R_PS2
#define PROP_SEG   (TQ_COUNT - SYNC_SEG - R_PS1 - R_PS2)

#if R_PS2 < 2
#error "PS2 must be at least 2 TQ"
#endif

#define R_CNF1  (R_BRP | ((SJW - 1) << 6))
#define R_CNF2  ((PROP_SEG - 1) | ((R_PS1 - 1) << 3) | (1 << SAM) | (1 << BTLMODE))
#define R_CNF3  (R_PS2 - 1)

/**************************************************************************

Die Datenübertragung über SPI zum MCP2515 darf nur während des Chip Selects
durchgeführt werden. Wird CS deaktiviert, muss die Datenübertragung entweder
sicher beendet sein (z.B. durch spi_getc() oder spi_readc()), oder es muss
nach spi_putc() auf Beendigung mittels WAIT_SPI gewartet werden. Das stellt
auch sicher, dass bei deaktiviertem CS keine Datenübertragung läuft. Daraus
ergibt sich auch, dass bei einem Aktivieren des CS nicht auf ein Ende der
Datenübertragung gewartet werden muss.

***************************************************************************/

#define MCP2515_SELECT     CLR_PORT_BIT(PORT_CS, P_CS)
#define MCP2515_DESELECT   SET_PORT_BIT(PORT_CS, P_CS)


static uint8_t multi_tx_buffer = 0;

static void mcp2515_select(uint8_t cmd)
{
	MCP2515_SELECT;
	SPDR = cmd;
}

static void mcp2515_deselect(void)
{
	WAIT_SPI;
	MCP2515_DESELECT;
}

/* MCP2515 datasheet, figure 12-2, page 65 */
static uint8_t mcp2515_read_register(uint8_t address)
{
	uint8_t data;

	mcp2515_select(SPI_READ);
	spi_putc(address);
	data = spi_readc();
	MCP2515_DESELECT;

	return data;
}

/* MCP2515 datasheet, figure 12-4, page 65 */
static void mcp2515_write_register( uint8_t address, uint8_t data )
{
	mcp2515_select(SPI_WRITE);
	spi_putc(address);
	spi_putc(data);
	mcp2515_deselect();
}

/* MCP2515 datasheet, figure 12-6, page 66 */
static void mcp2515_request_to_send(uint8_t buffer)
{
	mcp2515_select(SPI_RTS | (1 << buffer));
	mcp2515_deselect();
}

/* MCP2515 datasheet, figure 12-7, page 66 */
static void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	mcp2515_select(SPI_BIT_MODIFY);
	spi_putc(address);
	spi_putc(mask);
	spi_putc(data);
	mcp2515_deselect();
}

/* MCP2515 datasheet, figure 12-8, page 67 */
uint8_t mcp2515_read_status(void)
{
	uint8_t status;

	mcp2515_select(SPI_READ_STATUS);
	status = spi_readc();

	MCP2515_DESELECT;
	return status;
}

/* MCP2515 datasheet, figure 12-9, page 68 */
static uint8_t mcp2515_read_rx_status(void)
{
	uint8_t rx_status;

	mcp2515_select(SPI_RX_STATUS);
	rx_status = spi_readc();

	MCP2515_DESELECT;
	
	return rx_status;
}

static uint8_t mcp2515_set_mode(uint8_t mode)
{
	uint8_t canctrl;

	mcp2515_bit_modify( CANCTRL, MODE_MASK, mode);

	/* verify as advised in datasheet */
	canctrl = mcp2515_read_register( CANSTAT);
	canctrl &= MODE_MASK;

	return canctrl == mode;
}

static void mcp2515_reset(void)
{
	/*
	 * MCP2515 per Software Reset zuruecksetzten,
	 * danach ist der MCP2515 im Configuration Mode
	 */
	mcp2515_select(SPI_RESET);
	mcp2515_deselect();

	/*
	 * Mindestens 128 MCP Zyklen warten! Da der ATmega
	 * nur langsamer aber nie schneller sein kann, reicht
	 * das aus.
	 */
	for (uint8_t i = 0;i < 128;i++)
	{
		NOP;
	}
}

static void mcp2515_write_sid(uint8_t address, uint16_t id)
{
	mcp2515_select(SPI_WRITE);
	spi_putc(address);
	spi_putc(id >> 3);
	spi_putc(id << 5);
	spi_putc(0);
	spi_putc(0);
	mcp2515_deselect();
}

static void mcp2515_write_eid(uint8_t address, uint16_t id)
{
	mcp2515_select(SPI_WRITE);
	spi_putc(address);
	spi_putc(id >> 3);
	spi_putc((id << 5) | _BV(EXIDE));
	spi_putc(0);
	spi_putc(0);

	mcp2515_deselect();
}

uint8_t mcp2515_read_error_status(MCP2515_error_status *status)
{
	register uint8_t sreg = SREG;

	cli();
	status->tec  = mcp2515_read_register(TEC);
	status->rec  = mcp2515_read_register(REC);
	status->eflg = mcp2515_read_register(EFLG);
	SREG = sreg;

	return status->tec | status->rec | status->eflg;
}

void mcp2515_reset_overflow(uint8_t eflg)
{
	register uint8_t sreg = SREG;

	cli();
	mcp2515_bit_modify(CANINTF, _BV(RX1IF) | _BV(RX0IF), 0);
	mcp2515_write_register(EFLG, 0);
	SREG = sreg;
}

void mcp2515_dump_register(uint8_t *ptr)
{
	register uint8_t i;
	register uint8_t sreg = SREG;

	cli();
	mcp2515_select(SPI_READ);
	spi_putc(0);    /* Startadresse */
	spi_putc(0xff); /* Prefetch einleiten */

	for (i = 0;i < 128;i++)
	{
		*ptr++ = spi_prefetchc();
	}
	MCP2515_DESELECT;
	SREG = sreg;
}

uint8_t mcp2515_read_tx_input_pins(void)
{
	register uint8_t sreg = SREG;

	cli();
	register uint8_t result = (mcp2515_read_register(TXRTSCTRL) >> 3) & 0x7;
	SREG = sreg;
	return result;
}

void mcp2515_write_rx_output_pins(uint8_t state)
{
	register uint8_t sreg = SREG;

	cli();
	mcp2515_bit_modify(BFPCTRL, 3 << 4, state << 4);
	SREG = sreg;
}

void mcp2515_init(uint16_t id, uint8_t config_valid, uint8_t multi_tx)
{
	/* Flag, ob einer oder mehrere TX-Buffer benutzt werden sollen. */
	multi_tx_buffer = multi_tx;

	if (!config_valid)
	{
		id = UNDEFINED_SID;
	}

	/*
	 * Chip select (= SS) auf Ausgang noch bevor SPI
	 * initialisiert wird. Wichtig!
	 */
	SET_PORT_BIT(DDR_CS,  P_CS);
	SET_PORT_BIT(PORT_CS, P_CS);

	/* SPI Interface initialisieren. */
	spi_init();

	/* Reset MCP. Danach ist der MCP im Config Mode. */
	mcp2515_reset();

	if (!mcp2515_set_mode(MODE_CONFIG))
	{
		return;
	}

	/* Buffer 0 : Filter an, kein Rollover */
	mcp2515_write_register( RXB0CTRL, MCP_FILTER_BITS );

	/* Buffer 1 : Filter an, kein Rollover */
	mcp2515_write_register( RXB1CTRL, MCP_FILTER_BITS );

	/* ID 2047, Broadcast */
	if (id != GATEWAY_SID)
	{
		/* Receive buffer 0 */
		mcp2515_write_sid( RXF0SIDH, BROADCAST_SID);
		mcp2515_write_eid( RXF1SIDH, id);

		/* Receive buffer 1 */
		mcp2515_write_sid( RXF2SIDH, BROADCAST_SID);
		mcp2515_write_eid( RXF3SIDH, id);
		mcp2515_write_sid( RXF4SIDH, id);
		mcp2515_write_eid( RXF5SIDH, BROADCAST_SID);

		/*
		 * Ach so: Ein Extended Frame an die Broadcast Id
		 * macht keinen Sinn. Darum empfangen wir sie auch
		 * nicht.
		 */
	}
	else
	{
		/* Receive buffer 0 */
		 mcp2515_write_sid( RXF0SIDH, GATEWAY_SID);
		 mcp2515_write_eid( RXF1SIDH, GATEWAY_SID);

		/* Receive buffer 1 */
		 mcp2515_write_sid( RXF2SIDH, GATEWAY_SID);
		 mcp2515_write_eid( RXF3SIDH, GATEWAY_SID);
		 mcp2515_write_sid( RXF4SIDH, GATEWAY_SID);
		 mcp2515_write_eid( RXF5SIDH, GATEWAY_SID);

		/*
		 * Wir empfangen auf dem Gateway nur Nachrichten mit
		 * der Gateway ID. Obwohl keine Statndard Frames erwartet
		 * werden, sind sie trotzdem erlaubt.
		 */
	}

	/* Error Flags löschen */
	mcp2515_reset_overflow(_BV(RX1OVR) | _BV(RX0OVR));

	/* Standard ID ist komplette Maske */
	mcp2515_write_sid( RXM0SIDH, SID_MASK);
	mcp2515_write_sid( RXM1SIDH, SID_MASK);

	/* Bit timings konfigurieren */
	mcp2515_write_register( CNF1, R_CNF1 );
	mcp2515_write_register( CNF2, R_CNF2 );
	mcp2515_write_register( CNF3, R_CNF3 );

	/* Aktivieren der RXnBF pins als Output */
	mcp2515_write_register( BFPCTRL, _BV(B1BFE) | _BV(B0BFE) );

	/* TXnRTS Bits als Inputs schalten */
	mcp2515_write_register( TXRTSCTRL, 0 );

	/* CLKOUT aktivieren und Teiler einstellen */
	mcp2515_bit_modify( CANCTRL,
		_BV(CLKEN) | MCP_CLKOUT_MASK,
		_BV(CLKEN) | MCP_CLKOUT_Q1);

	/* So! Ab jetzt normal mode. */
	mcp2515_set_mode(MODE_NORMAL);

	/* Aktivieren der Rx Buffer Interrupts */
	mcp2515_write_register( CANINTE, _BV(RX0IE) | _BV(RX1IE) );

	/* Reset des Error Registers. */
	mcp2515_write_register(EFLG, 0);

	CLR_PORT_BIT(DDR_INT2,  P_INT2);
	CLR_PORT_BIT(PORT_INT2, P_INT2);

	/* ...und INT2 aktivieren. */
	CLR_PORT_BIT(GICR,  INT2); /* INT 2 disable */
	CLR_PORT_BIT(MCUCR, ISC2); /* Interrupt on falling edge */
	SET_PORT_BIT(GIFR,  INT2); /* Clear flag */
	SET_PORT_BIT(GICR,  INT2); /* INT 2 enable */
}

static int8_t can_get_free_buffer(void)
{
	uint8_t status = mcp2515_read_status();
	uint8_t buffer;
	
	/* Statusbyte:
	 *
	 * Bit  Funktion
	 *  2   TXB0CNTRL.TXREQ
	 *  4   TXB1CNTRL.TXREQ
	 *  6   TXB2CNTRL.TXREQ
	 */
	if (bit_is_clear(status, TX_STAT_TXB0))
	{
		buffer = 0;
	}
	else if ((multi_tx_buffer > MCP2515_SINGLE_TX_BUFFER) && bit_is_clear(status, TX_STAT_TXB1))
	{
		buffer = 1;
	}
	else if ((multi_tx_buffer > MCP2515_SINGLE_TX_BUFFER) && bit_is_clear(status, TX_STAT_TXB2))
	{
		buffer = 2;
	}
	else
	{
		buffer = -1;
	}
	return buffer;
}

/* MCP2515 datasheet, figure 12-5, page 66 */
int8_t can_put_msg(CAN_message *msg)
{
	uint8_t sreg = SREG;

	cli();
	int8_t buffer = can_get_free_buffer();
	
	if (buffer >= 0)
	{
		mcp2515_select(SPI_LOAD_TX | (0x2 * buffer));

		/* Standard ID einstellen */
		spi_putc(msg->sid >> 3);
		spi_putc((msg->sid << 5) | ((msg->status >> (FRAME_EXT - EXIDE)) & _BV(EXIDE)));

		/* Extended ID */
		spi_putc(msg->eid >> 8);
		spi_putc(msg->eid);

		uint8_t length = msg->length;

		/* Nachrichten Laenge einstellen */
		spi_putc(length);

		/* Daten */
		for (uint8_t i = 0; i < length; i++)
		{
			spi_putc(msg->data[i]);
		}
		mcp2515_deselect();

		/*
		 * CAN Nachricht verschicken
		 * die letzten drei Bit im RTS Kommando geben an, welcher
		 * Puffer gesendet werden soll.
		 */
		mcp2515_request_to_send(buffer);
	}
	SREG = sreg;
	return buffer;
}

int8_t can_send_msg(CAN_message *msg)
{
	int8_t buffer;
	int8_t retry = 15;

	do
	{
		buffer = can_put_msg(msg);
		if (buffer >= 0)
		{
			return buffer;
		}
		retry--;
		_delay_ms(1);
	}
	while (retry >= 0);

	return -2;
}

/* MCP2515 datasheet, figure 12-3, page 65 */
int8_t can_get_msg(CAN_message *msg)
{
	uint8_t sreg = SREG;
	uint8_t status;
	int8_t  buffer;
	
	cli();

	/* Status auslesen */
	status = mcp2515_read_rx_status();

	if (bit_is_set(status, RX_STAT_RXB0))
	{
		buffer = 0;
	}
	else if (bit_is_set(status, RX_STAT_RXB1))
	{
		buffer = 1;
	}
	else
	{
		buffer = -1;
	}

	if (buffer >= 0)
	{
		uint16_t id;

		mcp2515_select(SPI_READ_RX | (buffer << 2));

		/* Prefetch einleiten. */
		spi_putc(0x99); /* 10011001 */

		/* Standard ID auslesen */
		id = spi_prefetchc();
		msg->sid = (id << 3) | (spi_prefetchc() >> 5);
		
		/* Extended ID auslesen */
		id = spi_prefetchc();
		msg->eid = (id << 8) | spi_prefetchc();

		/* Laenge auslesen */
		uint8_t length = spi_prefetchc() & 0x0f;
		msg->length = length > 8 ? 8 : length;

		/* Daten auslesen */
		for (uint8_t i = 0; i < length; i++)
		{
			msg->data[i] = spi_prefetchc();
		}
		mcp2515_deselect();

		/* Interrupt Flag löschen */
		mcp2515_bit_modify(CANINTF, buffer == 0 ? _BV(RX0IF) : _BV(RX1IF), 0);
		
		buffer = (msg->status = status) & 0x7;
	}
	SREG = sreg;

	return buffer;
}
