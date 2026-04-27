/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Light dimming test firmware
**
*/

#ifndef LIGHT_H
#define LIGHT_H

#include "mrw.h"

extern void    simple_light_init(struct mrw_simple_light *dvc);
extern uint8_t simple_light_set_lightness(struct mrw_simple_light *dvc, uint8_t lightness);

extern void    light_init(struct mrw_light *dvc);
extern void    light_dimm(struct mrw_light *dvc);
extern uint8_t light_available(void);
extern void    light_set_lightness(struct mrw_light *dvc, uint8_t lightness);

extern const struct light_profile *get_light_profile(uint8_t type);

#endif
