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

/*******************************************************************
Dieser Zustandsautomat verarbeitet eingehende
CAN-Kommandos abhängig vom inneren
Betriebszustand des CAN-Knotens.
*******************************************************************/

#include <avr/io.h>

#include "CAN_Node.h"
#include "CAN_NodeCmd.h"
#include "CAN_NodeActivity.h"
#include "result_queue.h"
#include "tool.h"

/************************************************/
/* Vorwärtsdeklaration der choices Pseudostates */
/************************************************/

static int8_t checkIdButton(CAN_message *msg);
static int8_t checkId(CAN_message *msg);
static int8_t checkConfiguration(CAN_message *msg);
static int8_t checkPendingCommand(CAN_message *msg);
/**********************/
/* Der aktuelle State */
/**********************/

static int8_t state = CAN_NODE__INITIAL;

int8_t get_state_CAN_Node()
{
				return state;
}

/****************************************************/
/* Initialisierung und Verlassen des Initial States */
/****************************************************/

void init_CAN_Node(void)
{
				int8_t result;

				state = CAN_NODE__INITIAL;
				CAN_message *msg = null;

				result = initFirmware(msg);
				result = checkConfiguration(msg);
}

/************************/
/* Die Auswerteschleife */
/************************/
void CAN_Node(CAN_message *msg)
{
	int8_t   cmd     = msg->data[0];
	uint16_t unit_no = bit_is_set(msg->status, RX_STAT_FRAME_EXTENDED) ? msg->eid : 0;
	int8_t   result;

	result = check_command_CAN_Node(msg);
	if (result >= 0)
	{
		switch (state)
		{
/*******************************************************************
Dieser Zustand entspricht dem normalen Betriebsmodus. Hier werden
Steuerkommandos verarbeitet.
*******************************************************************/

		case CAN_NODE__OPERATING:
			switch(cmd)
			{
			case SETRGT:

				result = right(msg);
				break;

			case RESET:

				result = reset(msg);
				state = CAN_NODE__RESETTING;

				break;

			case PING:

				result = ping(msg);
				break;

			case SETLFT:

				result = left(msg);
				break;

			case SET_ID:

				result = checkIdButton(msg);
				break;

			case SETSGN:

				result = setSignal(msg);
				break;

			case SETRON:

				result = railOn(msg);
				break;

			case SETROF:

				result = railOff(msg);
				break;

			case QRYBUF:

				result = queryBufferState(msg);
				break;

			case QRYERR:

				result = queryErrorState(msg);
				break;

			case GETDIR:

				result = getDirection(msg);
				break;

			case GETRBS:

				result = getRailOccupation(msg);
				break;

			case CFGBGN:

				result = checkPendingCommand(msg);
				break;

			case GETVER:

				result = getVersion(msg);
				break;

			default:
				result = wrongOperatingTrigger(msg);
				break;
			}
			break;

/*******************************************************************
Im Konfigurationsmodus werden Konfigurationskommandos
verarbeitet. Dieser Zustand kann nur in einen Reset überführt
werden. Durch den Reset wird dann die hardware neu
initialisiert.
*******************************************************************/

		case CAN_NODE__CONFIGURING:
			switch(cmd)
			{
			case CFGEND:

				result = configEnd(msg);
				state = CAN_NODE__RESETTING;

				break;

			case PING:

				result = ping(msg);
				break;

			case CFGSWN:

				result = configSwitchNew(msg);
				break;

			case CFGSWO:

				result = configSwitchOld(msg);
				break;

			case CFGRAI:

				result = configRail(msg);
				break;

			case CFGMF2:

				result = configMainForm2(msg);
				break;

			case CFGMF3:

				result = configMainForm3(msg);
				break;

			case CFGPF2:

				result = configPreForm2(msg);
				break;

			case CFGPF3:

				result = configPreForm3(msg);
				break;

			case CFGPL2:

				result = configPreLight2(msg);
				break;

			case CFGML2:

				result = configMainLight2(msg);
				break;

			case CFGSL2:

				result = configRaillockLight2(msg);
				break;

			case CFGPL3:

				result = configPreLight3(msg);
				break;

			case CFGML3:

				result = configMainLight3(msg);
				break;

			case CFGML4:

				result = configMainLight4(msg);
				break;

			case RESET:

				result = reset(msg);
				state = CAN_NODE__RESETTING;

				break;

			default:
				result = wrongConfiguringTrigger(msg);
				break;
			}
			break;

/*******************************************************************
Dieser Zustand ist ein Übergangszustand. Es wurde ein Watchdog Reset
ausgelöst. Dieser wird erst nach einer Sekunde durchgeführt. Während
dessen ist der Mikrocontroller in diesem Zustand und kann keine Aufgaben
durchführen.
*******************************************************************/

		case CAN_NODE__RESETTING:
			switch(cmd)
			{
			case PING:

				result = ping(msg);
				break;

			case QRYBUF:

				result = queryBufferState(msg);
				break;

			case QRYERR:

				result = queryErrorState(msg);
				break;

			default:
				result = wrongResettingTrigger(msg);
				break;
			}
			break;

/*******************************************************************
Dieser Betriebsmodus wird nach dem Booten erreicht, wenn
keine gültige Konfiguration im EEPROM abgelegt ist. Von hier
aus kann nur konfiguriert oder gebootet werden.
*******************************************************************/

		case CAN_NODE__UNCONFIGURED:
			switch(cmd)
			{
			case CFGBGN:

				result = configBegin(msg);
				state = CAN_NODE__CONFIGURING;

				break;

			case PING:

				result = ping(msg);
				break;

			case RESET:

				result = reset(msg);
				state = CAN_NODE__RESETTING;

				break;

			case QRYERR:

				result = queryErrorState(msg);
				break;

			case QRYBUF:

				result = queryBufferState(msg);
				break;

			default:
				result = wrongUnconfiguredTrigger(msg);
				break;
			}
			break;

		}
	}
	if (result >= 0)
	{
		queue_result(cmd, unit_no, result);
	}
}

/****************/
/* Pseudostates */
/****************/

static int8_t checkIdButton(CAN_message *msg)
{
	int8_t result = -1;

	if (idButtonPressed(msg))
	{
				result = checkId(msg);
	}

	if (!idButtonPressed(msg))
	{
				result = idChangeDisabled(msg);
				state = CAN_NODE__OPERATING;

	}

	return result;
}

static int8_t checkId(CAN_message *msg)
{
	int8_t result = -1;

	if (!idChanged(msg))
	{
				result = idNotChanged(msg);
				state = CAN_NODE__OPERATING;

	}

	if (idChanged(msg))
	{
				result = setId(msg);
				state = CAN_NODE__RESETTING;

	}

	return result;
}

static int8_t checkConfiguration(CAN_message *msg)
{
	int8_t result = -1;

	if (!isConfigured(msg))
	{
				state = CAN_NODE__UNCONFIGURED;

	}

	if (isConfigured(msg))
	{
				state = CAN_NODE__OPERATING;

	}

	return result;
}

static int8_t checkPendingCommand(CAN_message *msg)
{
	int8_t result = -1;

	if (!isCommandPending(msg))
	{
				result = configBegin(msg);
				state = CAN_NODE__CONFIGURING;

	}

	if (isCommandPending(msg))
	{
				result = notConfiguring(msg);
				state = CAN_NODE__OPERATING;

	}

	return result;
}

