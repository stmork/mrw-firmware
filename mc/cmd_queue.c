/*
**
**	$Filename:	cmd_queue.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Modellrailway micro controller firmware
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

#include "firmware.h"
#include "cmd_queue.h"

#include <avr/interrupt.h>

#include "tool.h"

/**
 * The command queue for long term commands (form signals and switches)
 */
static          command     ring[RING_SIZE];
static volatile uint8_t     ring_start = 0;
static volatile uint8_t     ring_pos   = 0;
static volatile uint8_t     ring_size  = 0;


/**
 * Hier werden alle Langzeitkommandows eingereiht. Langzeitkommandos sind
 * alle Kommandos an Magnetartikel, weil deren Verarbeitung mit etwa einer
 * Sekunde veranschlagt werden. Der Interrupt muss für diese Routine
 * außerhalb gesperrt werden.
 */
uint8_t cmd_enqueue(mrw_device *dvc, uint8_t cmd, uint8_t code)
{
	uint8_t sreg = SREG;
	uint8_t result;

	cli();
	if (ring_size < RING_SIZE)
	{
		ring[ring_pos].device = dvc;
		ring[ring_pos].cmd    = cmd;
		ring[ring_pos].code   = code;
		ring_pos = (ring_pos + 1) & RING_MASK;
		ring_size++;

		result = MSG_QUEUED;
	}
	else
	{
		result = MSG_QUEUE_FULL;
	}
	SREG = sreg;

	return result;
}

/**
 * Hier wird das aktuelle Kommando aus dem Ring Buffer genommen.
 * Der Interrupt muss für diese Routine außerhalb gesperrt werden.
 */
void cmd_remove(void)
{
	uint8_t sreg = SREG;

	cli();
	if (ring_size > 0)
	{
		ring_size--;
		ring_start = (ring_start + 1) & RING_MASK;
	}
	SREG = sreg;
}

uint8_t cmd_ring_size(void)
{
	return ring_size;
}

int8_t cmd_find_device(mrw_device *dvc)
{
	for (uint8_t i = 0;i < ring_size;i++)
	{
		if (ring[(ring_start + i) & RING_MASK].device == dvc)
		{
			// There is a command already queued for that device.
			return i;
		}
	}
	return -1;
}

int8_t cmd_is_first(mrw_device *dvc)
{
	return (cmd_ring_size() > 0) && (dvc == ring[ring_start].device);
}

command *cmd_get_first(void)
{
	return ring_size > 0 ? &ring[ring_start] : null;
}
