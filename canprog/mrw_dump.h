/*
**
**	$Filename:	mrw_dump.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Dump clear text CAN frames.
**
**	Copyright (C) 2011 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**	which accompanies this distribution.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#ifndef MRW_DUMP_H
#define MRW_DUMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mrw.h"
#include "mcp2515.h"

extern void dump_can_msg(CAN_message *msg, unsigned char checksum, const char *comment);
extern void dump_mrw_msg(CAN_message *msg, uint8_t       checksum, const char *comment);

#ifdef __cplusplus
};
#endif

#endif
