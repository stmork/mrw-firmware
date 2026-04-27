/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Busy indicator
**
*/

#ifndef BUSY_H
#define BUSY_H

#include "bit.h"

#define PORT_BUSY  PORTB
#define DDR_BUSY   DDRB
#define P_BUSY     0

#define BUSY      SET_PORT_BIT(PORT_BUSY, P_BUSY)
#define IDLE      CLR_PORT_BIT(PORT_BUSY, P_BUSY)

#endif
