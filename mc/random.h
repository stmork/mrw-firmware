/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Compute random number
**
*/

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

extern void    random_preinit(void);
extern void    random_postinit(void);
extern uint8_t random_timer(void);

#endif
