/*
**
**	$Filename:	light_profile.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Light dimming profiles
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

#include "light_profile.h"

#include <avr/pgmspace.h>

const struct light_profile profiles[] =
{
};

uint8_t light_profile_count(void)
{
	return (sizeof(profiles) / sizeof(struct light_profile));
}
