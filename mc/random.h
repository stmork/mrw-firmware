/*
**
**	$Filename:	random.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Compute random number
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

#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

extern void    random_preinit(void);
extern void    random_postinit(void);
extern uint8_t random_timer(void);

#endif
