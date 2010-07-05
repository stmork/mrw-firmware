/*
**
**	$Filename:	rail.h $
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

#ifndef RAIL_H
#define RAIL_H

#include "mrw.h"

extern void    rail_on            (struct mrw_rail *dvc);
extern void    rail_off           (struct mrw_rail *dvc);
extern void    rail_init          (struct mrw_rail *dvc);
extern uint8_t rail_state_changed (struct mrw_rail *dvc);
extern uint8_t rail_occupied      (struct mrw_rail *dvc);

#endif

