/*
**
**	$Filename:	rail.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
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

#include "rail.h"
#include "bit.h"

void rail_init(struct mrw_rail *device)
{
	out_pin   (&device->power);
	init_input(&device->occupation, 1);
}

void rail_on(struct mrw_rail *device)
{
	set_pin(&device->power);
}

void rail_off(struct mrw_rail *device)
{
	clr_pin(&device->power);
}

uint8_t rail_state_changed(struct mrw_rail *device)
{
	return test_state_change(&device->occupation) != PIN_NO_CHANGE;
}

uint8_t rail_occupied(struct mrw_rail *device)
{
	return !device->occupation.state;
}
