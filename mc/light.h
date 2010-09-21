/*
**
**	$Filename:	light.h $
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

#ifndef LIGHT_H
#define LIGHT_H

#include "mrw.h"

extern void    light_init(struct mrw_light *dvc);
extern void    light_dimm(struct mrw_light *dvc);
extern uint8_t light_available(void);
extern void    light_set_lightness(struct mrw_light *dvc, uint8_t lightness);

#endif
