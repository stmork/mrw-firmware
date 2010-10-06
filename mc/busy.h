/*
**
**	$Filename:	busy.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Busy indicator
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

#ifndef BUSY_H
#define BUSY_H

#include "bit.h"

#define PORT_BUSY  PORTB
#define DDR_BUSY   DDRB
#define P_BUSY     0

#define BUSY      SET_PORT_BIT(PORT_BUSY, P_BUSY)
#define IDLE      CLR_PORT_BIT(PORT_BUSY, P_BUSY)

#endif
