/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	CAN message ring buffer
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
#define ring_has_overflow(ring) ((uint8_t)((ring)->size >= CAN_RING_SIZE))

extern void         ring_init(struct can_ring *ring);
extern CAN_message *ring_get_pos(struct can_ring *ring);
extern CAN_message *ring_get_start(struct can_ring *ring);
extern uint8_t      ring_increase(struct can_ring *ring);
extern void         ring_decrease(struct can_ring *ring);

#endif
