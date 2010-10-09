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
	dvc->on        =   0;
	dvc->counter   =   0;
	dvc->lightness = 255;
	dvc->profile   = get_light_profile(dvc->type);
	out_pin(&dvc->pin);
	set_dimm(dvc, 0);
	has_lights++;
}

uint8_t light_available(void)
{
	return has_lights;
}

static uint8_t light_is_on(struct mrw_light *dvc)
{
	return dvc->on;
}

void light_dimm(struct mrw_light *dvc)
{
	if (light_is_on(dvc))
	{
		if (dvc->profile->repeat)
		{
			dvc->counter++;
		}
		else if (dvc->counter < LIGHT_PROFILE_MASK)
		{
			dvc->counter++;
		}
		set_dimm(dvc, pgm_read_byte(&dvc->profile->values[dvc->counter & LIGHT_PROFILE_MASK]));
	}
}

void light_set_lightness(struct mrw_light *dvc, uint8_t lightness)
{
	if (lightness != dvc->lightness)
	{
		uint8_t turn_on  = dvc->threshold;
		uint8_t turn_off = dvc->threshold + 4;

		if ((dvc->lightness < turn_off) && (turn_off <= lightness) && light_is_on(dvc))
		{
			/* Zustandsübergang: Ausschalten */
			set_dimm(dvc, 0);
			dvc->counter = 0;
			dvc->on      = 0;
		}
		else if ((lightness < turn_on) && (turn_on <= dvc->lightness) && !light_is_on(dvc))
		{
			/* Zustandsübergang: Einschalten */
			dvc->counter = 0;
			dvc->on      = 1;
		}
		dvc->lightness = lightness;
	}
}
