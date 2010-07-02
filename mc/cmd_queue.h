/*
**
**	$Filename:	cmd_queue.h $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: cmd_queue.h 931 2010-04-14 08:39:15Z smork $
**
**	Modellrailway micro controller firmware
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

#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H

#include "mrw.h"

extern uint8_t  cmd_enqueue(mrw_device *dvc, uint8_t cmd, uint8_t code);
extern void     cmd_remove(void);
extern uint8_t  cmd_ring_size(void);
extern int8_t   cmd_find_device(mrw_device *dvc);
extern int8_t   cmd_is_first(mrw_device *dvc);
extern command *cmd_get_first(void);

#endif
