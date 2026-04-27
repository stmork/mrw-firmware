/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Light dimming profiles
**
*/

#ifndef LIGHT_PROFILE_H
#define LIGHT_PROFILE_H

#include <stdint.h>

#define LIGHT_PROFILE_SIZE 256
#define LIGHT_PROFILE_MASK (LIGHT_PROFILE_SIZE - 1)

struct light_profile
{
	const uint8_t  *values;
	uint8_t         repeat;
};

extern const struct light_profile profiles[];
extern uint8_t      light_profile_count(void);

#endif
