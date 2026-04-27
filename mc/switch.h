/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway micro controller
**
*/

#ifndef SWITCH_H
#define SWITCH_H

#include "mrw.h"

extern void    switch_init_old (struct mrw_switch *dvc);
extern void    switch_init_new (struct mrw_switch *dvc);
extern void    switch_off      (struct mrw_switch *dvc);
extern void    switch_left     (struct mrw_switch *dvc);
extern void    switch_right    (struct mrw_switch *dvc);
extern uint8_t switch_test     (struct mrw_switch *dvc);
extern uint8_t switch_dir      (struct mrw_switch *dvc);

extern void    form2_init      (struct mrw_form2 *signal);
extern uint8_t form2_turn      (struct mrw_form2 *signal, uint8_t cmd);
extern void    form2_off       (struct mrw_form2 *signal);

extern void    form3_init      (struct mrw_form3 *signal);
extern uint8_t form3_turn      (struct mrw_form3 *signal, uint8_t cmd);
extern void    form3_off       (struct mrw_form3 *signal);

#endif
