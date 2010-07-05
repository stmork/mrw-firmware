/*
**
**	$Filename:	can_processing_new.c  $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Neue CAN Message Verarbeitung ueber generiertes Zustandsdiagramm.
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

#include "can_processing.h"
#include "CAN_Node.h"

int8_t isConfiguring(void)
{
	return get_state_CAN_Node()  == CAN_NODE__CONFIGURING;
}

int8_t isResetting(void)
{
	return get_state_CAN_Node() == CAN_NODE__RESETTING;
}

void init_firmware(void)
{
	init_CAN_Node();
}

/**
 * Diese Routine verarbeitet eine einzelne CAN-Message.
 */
void process_can_message(CAN_message *msg)
{
	CAN_Node(msg);
}
