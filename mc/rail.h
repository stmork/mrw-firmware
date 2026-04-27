/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway micro controller
**
*/

#ifndef RAIL_H
#define RAIL_H

#include "mrw.h"

extern void    rail_on            (struct mrw_rail *dvc);
extern void    rail_off           (struct mrw_rail *dvc);
extern void    rail_init          (struct mrw_rail *dvc);
extern uint8_t rail_state_changed (struct mrw_rail *dvc);
extern uint8_t rail_occupied      (struct mrw_rail *dvc);

#endif

