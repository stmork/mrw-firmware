/*
**
**	$Filename:	can_processing_old.c  $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Alte CAN Message Verarbeitung.
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

#include "tool.h"
#include "can_processing.h"
#include "mrw.h"
#include "rail.h"
#include "switch.h"
#include "cmd_queue.h"
#include "result_queue.h"
#include "config.h"

static volatile uint8_t    wdc_lock      = FALSE;
static volatile uint8_t    config_mode   = FALSE;

int8_t isConfiguring(void)
{
	return config_mode;
}

int8_t isResetting(void)
{
	return wdc_lock;
}

int8_t isConfigured(void)
{
	return config.magic == CONFIG_MAGIC;
}

void init_firmware()
{
	wdc_lock = FALSE;
}

/**
 * Diese Routine verarbeitet eine einzelne CAN-Message.
 */
void process_can_message(CAN_message *msg)
{
	uint8_t  cmd = msg->data[0];

	if (cmd == PING)
	{
		queue_result(cmd, 0, MSG_OK);
		return;
	}
	else if (cmd == QRYBUF)
	{
		queue_infos(cmd, 0,
			wdc_lock ? MSG_RESET_PENDING : MSG_OK,
			rx_ring.size, tx_ring.size);
		return;
	}
	else if (cmd == QRYERR)
	{
		if (wdc_lock)
		{
			queue_result(cmd, 0, MSG_RESET_PENDING);
		}
		else
		{
			MCP2515_error_status status;

			mcp2515_read_error_status(&status);
			queue_infos(cmd, 0, MSG_OK, 0, status.eflg);
			queue_infos(cmd, 0, MSG_OK, 1, status.rec);
			queue_infos(cmd, 0, MSG_OK, 2, status.tec);
		}
		return;
	}
	else if (wdc_lock)
	{
		// block all commands on pending watch dog reset
		queue_result(cmd, 0, MSG_RESET_PENDING);
		return;
	}
	else if (cmd == RESET)
	{
		cli();
		queue_result(cmd, 0, MSG_OK);
		queue_result(cmd, 0, MSG_RESET_PENDING);
		flush_tx_ring();
		wdc_lock = TRUE;
		sei();
		return;
	}
	else if (((cmd & CMD_NEED_UNITNO) == 0) && (!bit_is_set(msg->status, RX_STAT_FRAME_EXTENDED)))
	{
		queue_result(cmd, 0, MSG_UNITNO_MISSING);
		return;
	}
	else if (cmd == SET_ID)
	{
		if (mcp2515_read_tx_input_pins() & CAN_ID_SWITCH)
		{
			queue_result(cmd, 0, MSG_ID_CHANGE_DISABLED);
		}
		else
		{
			uint16_t new_id = (uint16_t)msg->data[1] | ((uint16_t)msg->data[2] << 8);
			if (config.id != new_id)
			{
				cli();
				config.id     = new_id;
				config.count  = 0;
				config.magic  = 0;
				config_mode   = FALSE;
				save_eeprom_id(&config);
				queue_result( cmd, 0, MSG_OK);
				queue_result( cmd, 0, MSG_RESET_PENDING);

				flush_tx_ring();
				wdc_lock = TRUE;
				sei();
				return;
			}
			else
			{
				queue_result(cmd, 0, MSG_ID_NOT_CHANGED);
			}
		}
		return;
	}
	else if (cmd == CFGBGN)
	{
		if (cmd_ring_size() > 0)
		{
			queue_result(cmd, 0, MSG_PENDING);
			return;
		}
		config.count  = 0;
		config.magic  = 0;
		config_mode   = TRUE;
		queue_result(cmd, 0, MSG_OK);
		return;
	}
	else if (cmd == CFGEND)
	{
		cli();
		save_eeprom_config(&config);
		queue_info  (cmd, 0, MSG_OK, config.count);
		queue_result(cmd, 0, MSG_RESET_PENDING);

		flush_tx_ring();
		wdc_lock = TRUE;
		config_mode = FALSE;
		sei();
		return;
	}


	// extract device number
	uint16_t unit_no = msg->eid;
	if ((cmd & CAT_MASK) == CAT_CONFIG)
	{
		mrw_device *dvc = &config.dvc[config.count];

		if (!config_mode)
		{
			queue_result(cmd, unit_no, MSG_NOT_IN_CONFIG_MODE);
			return;
		}
		dvc->unit_no   = unit_no;
		switch (cmd)
		{
		case CFGSWN:
			dvc->unit_type = TYPE_SWITCH_NEW;
			config_connection( &dvc->unit.u_switch.left,  msg->data[1]);
			config_connection( &dvc->unit.u_switch.right, msg->data[2]);
			config_connection( &dvc->unit.u_switch.left_input.connection,  msg->data[3]);
			config_connection( &dvc->unit.u_switch.right_input.connection, msg->data[4]);
			config.count++;
			break;

		case CFGSWO:
			dvc->unit_type = TYPE_SWITCH_OLD;
			config_connection( &dvc->unit.u_switch.left,  msg->data[1]);
			config_connection( &dvc->unit.u_switch.right, msg->data[2]);
			config.count++;
			break;

		case CFGRAI:
			dvc->unit_type = TYPE_RAIL;
			config_connection( &dvc->unit.u_rail.power, msg->data[1]);
			config_connection( &dvc->unit.u_rail.occupation.connection, msg->data[2]);
			config.count++;
			break;

		case CFGPF2:
			dvc->unit_type = TYPE_SIGNAL_PF2;
			config_connection( &dvc->unit.u_form2.inductor[0], msg->data[1]);
			config_connection( &dvc->unit.u_form2.inductor[1], msg->data[2]);
			config.count++;
			break;
		case CFGMF2:
			dvc->unit_type = TYPE_SIGNAL_MF2;
			config_connection( &dvc->unit.u_form2.inductor[0], msg->data[1]);
			config_connection( &dvc->unit.u_form2.inductor[1], msg->data[2]);
			config.count++;
			break;

		case CFGPF3:
			dvc->unit_type = TYPE_SIGNAL_PF3;
			config_connection( &dvc->unit.u_form3.inductor[0], msg->data[1]);
			config_connection( &dvc->unit.u_form3.inductor[1], msg->data[2]);
			config_connection( &dvc->unit.u_form3.inductor[2], msg->data[3]);
			config.count++;
			break;
		case CFGMF3:
			dvc->unit_type = TYPE_SIGNAL_MF3;
			config_connection( &dvc->unit.u_form3.inductor[0], msg->data[1]);
			config_connection( &dvc->unit.u_form3.inductor[1], msg->data[2]);
			config_connection( &dvc->unit.u_form3.inductor[2], msg->data[3]);
			config.count++;
			break;

		case CFGPL2: // Vorsignal 2-begriffig
			init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_PL2);
			config.count++;
			break;
		case CFGML2: // Blocksignal (2-begriffig)
			init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_ML2);
			config.count++;
			break;
		case CFGSL2: // Gleissperrsignal
			init_light_signal(dvc, &msg->data[1], 2, TYPE_SIGNAL_SL2);
			config.count++;
			break;

		case CFGPL3: // Vorsignal 3-begriffig
			init_light_signal(dvc, &msg->data[1], 4, TYPE_SIGNAL_PL3);
			config.count++;
			break;
		case CFGML3: // Einfahrsignal (3-begriffig)
			init_light_signal(dvc, &msg->data[1], 3, TYPE_SIGNAL_ML3);
			config.count++;
			break;

		case CFGML4: // Ausfahrsignal (4-begriffig)
			init_light_signal(dvc, &msg->data[1], 5, TYPE_SIGNAL_ML4);
			config.count++;
			break;

		default:
			queue_result(cmd, unit_no, MSG_UNKNOWN_CMD);
			return;
		}

		queue_result(cmd, unit_no, MSG_OK); // handshaking!
		return;
	}
	else if (!isConfigured())
	{
		// this controller module not configured yet. inform caller.
		queue_result(cmd, 0, MSG_NOT_CONFIGURED_YET);
		return;
	}

	// Control request
	// Find device
	mrw_device *dvc = find_device(unit_no);
	if (dvc == null)
	{
		queue_result(cmd, unit_no, MSG_UNIT_NOT_FOUND);
		return;
	}

	// Check if command for that device is
	// already queued.
	if (cmd_find_device(dvc) >= 0)
	{
		// There is a command already queued for that device.
		queue_result(cmd, unit_no, MSG_PENDING);
		return;
	}

	// OK, ready for command dispatching
	switch (cmd)
	{
	case SETSGN:
		if ((dvc->unit_type == TYPE_SIGNAL_PF2) ||
			(dvc->unit_type == TYPE_SIGNAL_MF2) ||
			(dvc->unit_type == TYPE_SIGNAL_SF2) ||
			(dvc->unit_type == TYPE_SIGNAL_PF3) ||
			(dvc->unit_type == TYPE_SIGNAL_MF3))
		{
			queue_result(cmd, unit_no, cmd_enqueue(dvc, cmd, msg->data[1]));
		}
		else if ((dvc->unit_type == TYPE_SIGNAL_SL2) ||
				 (dvc->unit_type == TYPE_SIGNAL_PL2) ||
				 (dvc->unit_type == TYPE_SIGNAL_PL3) ||
				 (dvc->unit_type == TYPE_SIGNAL_ML2) ||
				 (dvc->unit_type == TYPE_SIGNAL_ML3) ||
				 (dvc->unit_type == TYPE_SIGNAL_ML4))
		{
			dvc->unit.u_signal.img = msg->data[1];
#if 0
			compute_signal(dvc);
			for (int q = 0;q < 8;q++)
			{
				queue_infos(cmd, unit_no, MSG_INFO, q, serial_buffer[q]);
			}
#endif
			queue_result(cmd, unit_no, MSG_OK);
		}
		else
		{
			queue_result(cmd, unit_no, MSG_UNITTYPE_WRONG);
		}
		return;

	case SETLFT:
		if ((dvc->unit_type == TYPE_SWITCH_NEW) && (switch_dir(&dvc->unit.u_switch) == SWITCH_STATE_LEFT))
		{
			queue_result(cmd, unit_no, MSG_OK);
		}
		else
		{
			queue_result(cmd, unit_no, cmd_enqueue(dvc, cmd, 0));
		}
		return;
	case SETRGT:
		if ((dvc->unit_type == TYPE_SWITCH_NEW) && (switch_dir(&dvc->unit.u_switch) == SWITCH_STATE_RIGHT))
		{
			queue_result(cmd, unit_no, MSG_OK);
		}
		else
		{
			queue_result(cmd, unit_no, cmd_enqueue(dvc, cmd, 0));
		}
		return;

	case SETRON:
		rail_on(&dvc->unit.u_rail);
		break;

	case SETROF:
		rail_off(&dvc->unit.u_rail);
		break;

	case GETDIR:
		if (dvc->unit_type == TYPE_SWITCH_NEW)
		{
			queue_info(cmd, unit_no, MSG_OK, switch_dir(&dvc->unit.u_switch));
		}
		else
		{
			queue_result(cmd, unit_no, MSG_UNITTYPE_WRONG);
		}
		return;

	case GETRBS:
		queue_info(cmd, unit_no, MSG_OK, rail_occupied(&dvc->unit.u_rail));
		return;

	case FLASH_REQ:
	case FLASH_DATA:
	case FLASH_CHECK:
		queue_result(cmd, 0, MSG_IGNORED);
		return;

	default:
		queue_result(cmd, 0, MSG_UNKNOWN_CMD);
		return;
	}

	queue_result(cmd, unit_no, MSG_OK);
}

