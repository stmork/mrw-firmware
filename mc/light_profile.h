/*
**
**	$Filename:	light_profile.h $
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

extern const struct light_profile *get_light_profile(uint8_t type);

#endif
