/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway micro controller firmware
**
*/

#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H

#include "mrw.h"

extern uint8_t  cmd_enqueue(mrw_device *dvc, uint8_t cmd, uint8_t code);
extern void     cmd_remove(void);
extern uint8_t  cmd_ring_size(void);
extern int8_t   cmd_find_device(mrw_device *dvc);
extern int8_t   cmd_is_first(mrw_device *dvc);
extern command *cmd_get_first(void);

#endif
