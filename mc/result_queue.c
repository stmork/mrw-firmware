/*
**
**	$Filename:	result_queue.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Verwaltung der CAN-Antwort Messages.
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

#include "result_queue.h"
#include "can_ring.h"
#include "config.h"

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <util/delay.h>

struct can_ring rx_ring;
struct can_ring tx_ring;

/**
 * Hier werden Antworten auf eingehende CAN-Messages in einen
 * Ring Buffer eingereiht. Die Antworten gehen alle an das
 * CAN-RS232-Gateway. In die Nachricht wird die eigene Controller-ID
 * sowie wenn vorhanden die Unitnumber eingetragen. Im Kommandobyte
 * wird noch das Ergbnisbit gesetzt und der Verarbeitungszustand
 * untergebracht. Der Ring Buffer wird im Hauptprogramm verarbeitet.
 * Der Interrupt muss für diese Routine außerhalb gesperrt werden.
 */
static void fill_message(
	CAN_message *msg,
	uint8_t  cmd,
	uint16_t unit_no,
	uint8_t  code)
{
	msg->sid     = GATEWAY_SID;
	msg->eid     = config.id;
	msg->status  = _BV(FRAME_EXT);

	msg->length  = 0;
	msg->data[msg->length++] = cmd | MSG_RESULT;
	msg->data[msg->length++] = code;
	msg->data[msg->length++] = config.id & 0xff ;
	msg->data[msg->length++] = config.id >> 8;
	msg->data[msg->length++] = unit_no & 0xff ;
	msg->data[msg->length++] = unit_no >> 8;
}

static void bump_ring(void)
{
	if (ring_increase(&tx_ring))
	{
		CAN_message *msg = ring_get_start(&tx_ring);

		while(can_put_msg(msg) < 0);
		ring_decrease(&tx_ring);
	}
}

void queue_result(
	uint8_t  cmd,
	uint16_t unit_no,
	uint8_t  code)
{
	CAN_message *msg = ring_get_pos(&tx_ring);

	fill_message(msg, cmd, unit_no, code);
	bump_ring();
}

void queue_info(
	uint8_t  cmd,
	uint16_t unit_no,
	uint8_t  code,
	uint8_t  info)
{
	CAN_message *msg = ring_get_pos(&tx_ring);

	fill_message(msg, cmd, unit_no, code);
	msg->data[msg->length++] = info;
	bump_ring();
}

void queue_infos(
	uint8_t  cmd,
	uint16_t unit_no,
	uint8_t  code,
	uint8_t  info1,
	uint8_t  info2)
{
	CAN_message *msg = ring_get_pos(&tx_ring);

	fill_message(msg, cmd, unit_no, code);
	msg->data[msg->length++] = info1;
	msg->data[msg->length++] = info2;
	bump_ring();
}

/**
 * Diese Methode entleert den CAN Sendepuffer. die Routine
 * wird erst verlassen, wenn keine CAN Message mehr im Rinpuffer
 * verblieben ist.
 */
void flush_tx_ring(void)
{
	while (ring_has_messages(&tx_ring))
	{
		CAN_message *msg = ring_get_start(&tx_ring);

		if (can_put_msg(msg) >= 0)
		{
			ring_decrease(&tx_ring);
		}
	}

	while (mcp2515_read_status() & (TX_STAT_TXB0|TX_STAT_TXB1|TX_STAT_TXB2))
	{
		_delay_ms(1);
	}
}
