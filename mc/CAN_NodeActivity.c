/*
**
**	$Filename:	CAN_NodeActivity.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Modellrailway state chart generation CAN-Node.
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

#include <avr/interrupt.h>

#include "CAN_NodeActivity.h"

#include "tool.h"
#include "firmware.h"
#include "eeprom.h"
#include "result_queue.h"
#include "cmd_queue.h"
#include "config.h"
#include "switch.h"
#include "light.h"
#include "rail.h"
#include "signal.h"
#include "revision.h"

int8_t check_command_CAN_Node(CAN_message *msg)
{
	return
		((msg->data[0] & CMD_NEED_UNITNO) == 0) && (!bit_is_set(msg->status, RX_STAT_FRAME_EXTENDED)) ?
		MSG_UNITNO_MISSING : MSG_OK;
}

int8_t initFirmware(CAN_message *msg)
{
	return getVersion(msg);
}

/*******************/
/* Config commands */
/*******************/

/* CFGBGN */
int8_t  configBegin(CAN_message *msg)
{
	config.count  = 0;
	config.magic  = 0;

	return MSG_OK;
}

int8_t notConfiguring(CAN_message *msg)
{
	return MSG_PENDING;
}

/* CFGEND */
int8_t  configEnd(CAN_message *msg)
{
	uint8_t  cmd = msg->data[0];

	config_sort();

	cli();
	save_eeprom_config(&config);
	queue_info  (cmd, 0, MSG_OK, config.count);
	queue_result(cmd, 0, MSG_RESET_PENDING);

	flush_tx_ring();
	sei();

	return NO_RESULT;
}

/* CFGMF2 */
int8_t  configMainForm2(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SIGNAL_MF2;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_form2.inductor[0], msg->data[1]);
	config_connection( &dvc->unit.u_form2.inductor[1], msg->data[2]);
	config.count++;

	return MSG_OK;
}

/* CFGMF3 */
int8_t  configMainForm3(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SIGNAL_MF3;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_form3.inductor[0], msg->data[1]);
	config_connection( &dvc->unit.u_form3.inductor[1], msg->data[2]);
	config_connection( &dvc->unit.u_form3.inductor[2], msg->data[3]);
	config.count++;

	return MSG_OK;
}

/* CFGML2 */
int8_t  configMainLight2(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_ML2);
	config.count++;

	return MSG_OK;
}

/* CFGML3 */
int8_t  configMainLight3(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 3, TYPE_SIGNAL_ML3);
	config.count++;

	return MSG_OK;
}

/* CFGML4 */
int8_t  configMainLight4(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 5, TYPE_SIGNAL_ML4);
	config.count++;

	return MSG_OK;
}

/* CFGPF2 */
int8_t  configPreForm2(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SIGNAL_PF2;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_form2.inductor[0], msg->data[1]);
	config_connection( &dvc->unit.u_form2.inductor[1], msg->data[2]);
	config.count++;

	return MSG_OK;
}

/* CFGPF3 */
int8_t  configPreForm3(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SIGNAL_PF3;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_form3.inductor[0], msg->data[1]);
	config_connection( &dvc->unit.u_form3.inductor[1], msg->data[2]);
	config_connection( &dvc->unit.u_form3.inductor[2], msg->data[3]);
	config.count++;

	return MSG_OK;
}

/* CFGPL2 */
int8_t  configPreLight2(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_PL2);
	config.count++;

	return MSG_OK;
}

/* CFGPL3 */
int8_t  configPreLight3(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 4, TYPE_SIGNAL_PL3);
	config.count++;

	return MSG_OK;
}

/* CFGRAI */
int8_t  configRail(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_RAIL;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_rail.power, msg->data[1]);
	config_connection( &dvc->unit.u_rail.occupation.connection, msg->data[2]);
	config.count++;

	return MSG_OK;
}

/* CFGSL2 */
int8_t  configRaillockLight2(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_no   = msg->eid;
	init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_SL2);
	config.count++;

	return MSG_OK;
}

/* CFGSWN */
int8_t  configSwitchNew(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SWITCH_NEW;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_switch.left,  msg->data[1]);
	config_connection( &dvc->unit.u_switch.right, msg->data[2]);
	config_connection( &dvc->unit.u_switch.left_input.connection,  msg->data[3]);
	config_connection( &dvc->unit.u_switch.right_input.connection, msg->data[4]);
	config.count++;

	return MSG_OK;
}

/* CFGSWO */
int8_t  configSwitchOld(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	dvc->unit_type = TYPE_SWITCH_OLD;
	dvc->unit_no   = msg->eid;
	config_connection( &dvc->unit.u_switch.left,  msg->data[1]);
	config_connection( &dvc->unit.u_switch.right, msg->data[2]);
	config.count++;

	return MSG_OK;
}

/* CFGLGT */
int8_t  configLight(CAN_message *msg)
{
	mrw_device *dvc = &config.dvc[config.count];

	if (msg->length >= 4)
	{
		dvc->unit_type = TYPE_LIGHT;
		dvc->unit_no   = msg->eid;
		config_connection(&dvc->unit.u_light.pin, msg->data[1]);
		dvc->unit.u_light.threshold = msg->data[2];
		dvc->unit.u_light.type      = msg->data[3];
	}
	else
	{
		dvc->unit_type = TYPE_SIMPLE_LIGHT;
		dvc->unit_no   = msg->eid;
		dvc->unit.u_simple_light.threshold = msg->data[2];
		dvc->unit.u_simple_light.byte = msg->data[1] >> 3;
		dvc->unit.u_simple_light.bit  = msg->data[1] & 0x7;
	}
	config.count++;

	return MSG_OK;
}

/* QRYBUF */
int8_t  queryBufferState(CAN_message *msg)
{
	queue_infos2(msg->data[0], 0,
		MSG_OK,
		rx_ring.size, tx_ring.size);

	return NO_RESULT;
}

/* QRYERR */
int8_t  queryErrorState(CAN_message *msg)
{
	MCP2515_error_status status;
	int8_t               cmd = msg->data[0];

	mcp2515_read_error_status(&status);

	queue_infos4(cmd, 0, MSG_OK, 3,
		status.eflg, status.rec, status.tec);

	return NO_RESULT;
}

/******************/
/* Query commands */
/******************/

/* GETDIR */
int8_t  getDirection(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);
	int8_t      cmd = msg->data[0];

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	else if (dvc->unit_type == TYPE_SWITCH_NEW)
	{
		queue_info(cmd, msg->eid, MSG_OK, switch_dir(&dvc->unit.u_switch));
		return NO_RESULT;
	}
	else
	{
		return MSG_UNITTYPE_WRONG;
	}
}

/* GETRBS */
int8_t  getRailOccupation(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	else
	{
		queue_info(msg->data[0], msg->eid, MSG_OK, rail_occupied(&dvc->unit.u_rail));
	}
	return NO_RESULT;
}

/**********************/
/* Switching commands */
/**********************/

/* SETLFT */
int8_t  left(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);
	int8_t      cmd = msg->data[0];

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	else if ((dvc->unit_type == TYPE_SWITCH_NEW) && (switch_dir(&dvc->unit.u_switch) == SWITCH_STATE_LEFT))
	{
		return MSG_OK;
	}
	else
	{
		return cmd_enqueue(dvc, cmd, 0);
	}
}

/* SETRGT */
int8_t  right(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);
	int8_t      cmd = msg->data[0];

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	else if ((dvc->unit_type == TYPE_SWITCH_NEW) && (switch_dir(&dvc->unit.u_switch) == SWITCH_STATE_RIGHT))
	{
		return MSG_OK;
	}
	else
	{
		return cmd_enqueue(dvc, cmd, 0);
	}
}

/* SETROF */
int8_t  railOff(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	rail_off(&dvc->unit.u_rail);
	return MSG_OK;
}

/* SETRON */
int8_t  railOn(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	rail_on(&dvc->unit.u_rail);

	return MSG_OK;
}

/* SETSGN */
int8_t setSignal(CAN_message *msg)
{
	mrw_device *dvc = find_device(msg->eid);
	int8_t      cmd = msg->data[0];

	if (dvc == null)
	{
		return MSG_UNIT_NOT_FOUND;
	}
	else if ((dvc->unit_type == TYPE_SIGNAL_PF2) ||
		(dvc->unit_type == TYPE_SIGNAL_MF2) ||
		(dvc->unit_type == TYPE_SIGNAL_SF2) ||
		(dvc->unit_type == TYPE_SIGNAL_PF3) ||
		(dvc->unit_type == TYPE_SIGNAL_MF3))
	{
		return cmd_enqueue(dvc, cmd, msg->data[1]);
	}
	else if ((dvc->unit_type == TYPE_SIGNAL_SL2) ||
			 (dvc->unit_type == TYPE_SIGNAL_PL2) ||
			 (dvc->unit_type == TYPE_SIGNAL_PL3) ||
			 (dvc->unit_type == TYPE_SIGNAL_ML2) ||
			 (dvc->unit_type == TYPE_SIGNAL_ML3) ||
			 (dvc->unit_type == TYPE_SIGNAL_ML4))
	{
		dvc->unit.u_signal.img = msg->data[1];
		compute_signal(dvc);
		return MSG_OK;
	}
	return MSG_UNITTYPE_WRONG;
}

/* SENSOR */
int8_t sensor(CAN_message *msg)
{
	if (msg->data[1] == SENSOR_LIGHT)
	{
		if (msg->status & (1 << FRAME_EXT))
		{
			mrw_device *dvc = find_device(msg->eid);

			if (dvc == null)
			{
				return MSG_UNIT_NOT_FOUND;
			}
			else if (dvc->unit_type == TYPE_LIGHT)
			{
				light_set_lightness(&dvc->unit.u_light, msg->data[2]);
				queue_infos3(msg->data[0], msg->eid, MSG_OK, msg->data[1], msg->data[2], 1);
			}
			else if (dvc->unit_type == TYPE_SIMPLE_LIGHT)
			{
				simple_light_set_lightness(&dvc->unit.u_simple_light, msg->data[2]);
				queue_infos3(msg->data[0], msg->eid, MSG_OK, msg->data[1], msg->data[2], 1);
			}
			else
			{
				return MSG_UNITTYPE_WRONG;
			}
		}
		else
		{
			uint8_t i, lights = 0;

			for (i = 0; i < config.count; i++)
			{
				if (config.dvc[i].unit_type == TYPE_LIGHT)
				{
					light_set_lightness(&config.dvc[i].unit.u_light, msg->data[2]);
					lights++;
				}
				else if (config.dvc[i].unit_type == TYPE_SIMPLE_LIGHT)
				{
					simple_light_set_lightness(&config.dvc[i].unit.u_simple_light, msg->data[2]);
					lights++;
				}
			}
			if (lights > 0)
			{
				queue_infos3(msg->data[0], 0, MSG_OK, msg->data[1], msg->data[2], lights);
			}
		}
	}
	return NO_RESULT;
}

/*************************/
/* Maintainance commands */
/*************************/

/* PING */
int8_t  ping(CAN_message *msg)
{
	return MSG_OK;
}

/* RESET */
int8_t  reset(CAN_message *msg)
{
	int8_t cmd = msg->data[0];

	cli();
	queue_result(cmd, 0, MSG_OK);
	queue_result(cmd, 0, MSG_RESET_PENDING);
	flush_tx_ring();
	sei();

	return NO_RESULT;
}

/* GETVER */
int8_t getVersion(CAN_message *msg)
{
	int8_t cmd = GETVER;

	queue_infos4(cmd, 0, MSG_OK, 3,
		FIRMWARE_VERSION,
		FIRMWARE_REVISION & 0xff,
		FIRMWARE_REVISION >> 8);
	
	return NO_RESULT;
}

/* SET_ID */
int8_t  idNotChanged(CAN_message *msg)
{
	return MSG_ID_NOT_CHANGED;
}

/* SET_ID */
int8_t idChangeDisabled(CAN_message *msg)
{
	return MSG_ID_CHANGE_DISABLED;
}

/* SET_ID */
int8_t  setId(CAN_message *msg)
{
	int8_t   cmd    = msg->data[0];
	uint16_t new_id = (uint16_t)msg->data[1] | ((uint16_t)msg->data[2] << 8);

	cli();
	config.id     = new_id;
	config.count  = 0;
	save_eeprom_id(&config);
	queue_result( cmd, 0, MSG_OK);
	queue_result( cmd, 0, MSG_RESET_PENDING);
	flush_tx_ring();
	sei();

	return NO_RESULT;
}

/*********************/
/* Transition guards */
/*********************/

int8_t idButtonPressed(CAN_message *msg)
{
	return (mcp2515_read_tx_input_pins() & CAN_ID_SWITCH) == 0;
}

int8_t idChanged(CAN_message *msg)
{
	uint16_t new_id = (uint16_t)msg->data[1] | ((uint16_t)msg->data[2] << 8);

	return config.id != new_id;
}

int8_t isConfigured(CAN_message *msg)
{
	return config.magic == CONFIG_MAGIC;
}

int8_t isCommandPending(CAN_message *msg)
{
	return cmd_ring_size() > 0;
}

/********************/
/* Illegal commands */
/********************/ 

int8_t wrongOperatingTrigger(CAN_message *msg)
{
	int8_t cmd = msg->data[0];

	if (msg->length == 0)
	{
		return MSG_IGNORED;
	}
	else if ((
		(cmd & CAT_MASK) == CAT_CONFIG) ||
		(cmd == CFGEND))
	{
		return MSG_NOT_IN_CONFIG_MODE;
	}
	else if (
		(cmd == FLASH_REQ) ||
		(cmd == FLASH_DATA) ||
		(cmd == FLASH_CHECK))
	{
		return MSG_IGNORED;
	}
	return MSG_UNKNOWN_CMD;
}

int8_t wrongConfiguringTrigger(CAN_message *msg)
{
	return MSG_IGNORED;
}

int8_t wrongResettingTrigger(CAN_message *msg)
{
	return MSG_RESET_PENDING;
}

int8_t wrongUnconfiguredTrigger(CAN_message *msg)
{
	return MSG_NOT_CONFIGURED_YET;
}
