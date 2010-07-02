/*
**
**	$Filename:	mrw.h $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: mrw_dump.h 931 2010-04-14 08:39:15Z smork $
**
**	Dump clear text CAN frames.
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

#ifndef MRW_DUMP_H
#define MRW_DUMP_H

#include "mrw.h"
#include "mcp2515.h"

extern void dump_can_msg(CAN_message *msg, unsigned char checksum, char *comment);
extern void dump_mrw_msg(CAN_message *msg, uint8_t       checksum, char *comment);

#endif
