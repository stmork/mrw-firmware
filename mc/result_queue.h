/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Verwaltung der CAN-Antwort Messages.
**
*/

#ifndef RESULT_QUEUE_H
#define RESULT_QUEUE_H

#include <stdint.h>
#include "can_ring.h"

extern struct can_ring rx_ring;
extern struct can_ring tx_ring;       

extern void queue_result(uint8_t cmd, uint16_t unit_no, uint8_t code);
extern void queue_info  (uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info);
extern void queue_infos2(uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info1, uint8_t info2);
extern void queue_infos3(uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info1, uint8_t info2, uint8_t info3);
extern void queue_infos4(uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info1, uint8_t info2, uint8_t info3, uint8_t info4);
extern void flush_tx_ring(void);

#endif
