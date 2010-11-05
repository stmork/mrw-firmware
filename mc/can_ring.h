/*
**
**	$Filename:	can_ring.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	CAN message ring buffer
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

#ifndef CAN_RING_H
#define CAN_RING_H

#include "mcp2515.h"

#define CAN_RING_SIZE 32
#define CAN_RING_MASK (CAN_RING_SIZE - 1)

struct can_ring
{
	CAN_message buffer[CAN_RING_SIZE];
	volatile uint8_t     start;
	volatile uint8_t     pos;
	volatile uint8_t     size;
	volatile uint16_t    count;
};

#define ring_has_messages(ring) ((ring)->size)

extern void         ring_init(struct can_ring *ring);
extern CAN_message *ring_get_pos(struct can_ring *ring);
extern CAN_message *ring_get_start(struct can_ring *ring);
extern uint8_t      ring_increase(struct can_ring *ring);
extern void         ring_decrease(struct can_ring *ring);
extern uint8_t      ring_has_overflow(struct can_ring *ring);

#endif
