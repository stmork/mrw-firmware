/*
**
**	$Filename:	pwm.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	PWM soft handler
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

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#include "mrw.h"

#define IS_PWM_DIMM(dvc) (((dvc)->dimm > 0) && ((dvc)->dimm < PWM_MAX))
#define _IS_PWM_DIMM(dvc) 1

extern void set_dimm(struct mrw_light *dvc, uint8_t value);
extern void handle_pwm(struct mrw_light *dvc);

#endif
