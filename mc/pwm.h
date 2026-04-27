/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	PWM soft handler
**
*/

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#include "mrw.h"

#define IS_PWM_DIMM(dvc) (((dvc)->dimm > 0) && ((dvc)->dimm < PWM_MAX))
#define _IS_PWM_DIMM(dvc) 1

extern void set_dimm(struct mrw_light *dvc, uint8_t value);
extern void handle_pwm(struct mrw_light *dvc);

#endif
