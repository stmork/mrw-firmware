/*
**
**	$Id$
**	$Revision$
**	$Date$
**	$Author$
**
**	Modellrailway state chart generation CAN-Node.
**
**	Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#ifndef CAN_NODE_ACTIVITY_H
#define CAN_NODE_ACTIVITY_H

#include <inttypes.h>
#include "mcp2515.h"

/*************************/
/* Transition activities */
/*************************/

extern int8_t configBegin(CAN_message *msg);
extern int8_t configEnd(CAN_message *msg);
extern int8_t configLight(CAN_message *msg);
extern int8_t configMainForm2(CAN_message *msg);
extern int8_t configMainForm3(CAN_message *msg);
extern int8_t configMainLight2(CAN_message *msg);
extern int8_t configMainLight3(CAN_message *msg);
extern int8_t configMainLight4(CAN_message *msg);
extern int8_t configPreForm2(CAN_message *msg);
extern int8_t configPreForm3(CAN_message *msg);
extern int8_t configPreLight2(CAN_message *msg);
extern int8_t configPreLight3(CAN_message *msg);
extern int8_t configRail(CAN_message *msg);
extern int8_t configRaillockLight2(CAN_message *msg);
extern int8_t configSwitchNew(CAN_message *msg);
extern int8_t configSwitchOld(CAN_message *msg);
extern int8_t getDirection(CAN_message *msg);
extern int8_t getRailOccupation(CAN_message *msg);
extern int8_t getVersion(CAN_message *msg);
extern int8_t idChangeDisabled(CAN_message *msg);
extern int8_t idNotChanged(CAN_message *msg);
extern int8_t initFirmware(CAN_message *msg);
extern int8_t left(CAN_message *msg);
extern int8_t notConfiguring(CAN_message *msg);
extern int8_t ping(CAN_message *msg);
extern int8_t queryBufferState(CAN_message *msg);
extern int8_t queryErrorState(CAN_message *msg);
extern int8_t railOff(CAN_message *msg);
extern int8_t railOn(CAN_message *msg);
extern int8_t reset(CAN_message *msg);
extern int8_t right(CAN_message *msg);
extern int8_t setId(CAN_message *msg);
extern int8_t setSignal(CAN_message *msg);
/*********************/
/* Transition guards */
/*********************/

extern int8_t idButtonPressed(CAN_message *msg);
extern int8_t idChanged(CAN_message *msg);
extern int8_t isCommandPending(CAN_message *msg);
extern int8_t isConfigured(CAN_message *msg);
/********************/
/* Illegal commands */
/********************/

extern int8_t wrongOperatingTrigger(CAN_message *msg);
extern int8_t wrongConfiguringTrigger(CAN_message *msg);
extern int8_t wrongResettingTrigger(CAN_message *msg);
extern int8_t wrongUnconfiguredTrigger(CAN_message *msg);
/*******************************/
/* check each incoming command */
/*******************************/
extern int8_t check_command_CAN_Node(CAN_message *msg);
#endif
