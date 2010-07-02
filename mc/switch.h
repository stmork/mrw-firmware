/*
**
**	$Filename:	switch.h $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: switch.h 931 2010-04-14 08:39:15Z smork $
**
**	Modellrailway micro controller
**
**	Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**	which accompanies this distribution.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
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
