/*
**
**	$Filename:	result_queue.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Verwaltung der CAN-Antwort Messages.
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

#ifndef RESULT_QUEUE_H
#define RESULT_QUEUE_H

#include <stdint.h>
#include "can_ring.h"

extern struct can_ring rx_ring;
extern struct can_ring tx_ring;       

extern void queue_result(uint8_t cmd, uint16_t unit_no, uint8_t code);
extern void queue_info  (uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info);
extern void queue_infos (uint8_t cmd, uint16_t unit_no, uint8_t code, uint8_t info1, uint8_t info2);
extern void flush_tx_ring(void);

#endif
