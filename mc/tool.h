/*
**
**	$Filename:	tool.h $
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

#ifndef TOOL_H
#define TOOL_H

#ifndef null
#define null 0
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifndef TRUE
#define TRUE 1
#endif

#ifndef NOP
#define NOP    { asm volatile ("nop"); }
#endif


#endif
