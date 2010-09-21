/*
**
**	$Filename:	light.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Light dimming test firmware
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

#include <avr/pgmspace.h>

#include "light.h"
#include "bit.h"
#include "light_profile.h"
#include "pwm.h"

static uint8_t has_lights = 0;

void light_init(struct mrw_light *dvc)
{
	dvc->counter = 0;
	dvc->profile = &profiles[has_lights++];
	out_pin(&dvc->pin);
	set_dimm(dvc, 0);
}

void light_dimm(struct mrw_light *dvc)
{
	set_dimm(dvc, pgm_read_byte(&dvc->profile->profile[dvc->counter & LIGHT_PROFILE_MASK]));
	dvc->counter++;
}

uint8_t light_available(void)
{
	return has_lights;
}
