/*
**
**	$Filename:	mrw.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
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

#include "mrw_dump.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#ifdef USE_SID_IN_RESULT
#define IDX_INFO_START 6
#else
#define IDX_INFO_START 4
#endif


struct string_map
{
	uint8_t  code;
	char    *text;
};

struct string_map cmd_map[] =
{
	{ SETLFT, "SETLFT" },
	{ SETRGT, "SETRGT" },
	{ GETDIR, "GETDIR" },

	{ SETRON, "SETRON" },
	{ SETROF, "SETROF" },
	{ GETRBS, "GETRBS" },

	{ SETSGN, "SETSGN" },

	{ CFGSWN, "CFGSWN"},
	{ CFGSWO, "CFGSWO"},
	{ CFGRAI, "CFGRAI"},
	{ CFGPF2, "CFGPF2"},
	{ CFGPF3, "CFGPF3"},
	{ CFGMF2, "CFGMF2"},
	{ CFGMF3, "CFGMF3"},
	{ CFGPL2, "CFGPL2"},
	{ CFGPL3, "CFGPL3"},
	{ CFGSL2, "CFGSL2"},
	{ CFGML2, "CFGML2"},
	{ CFGML3, "CFGML3"},
	{ CFGML4, "CFGML4"},
	{ CFGLGT, "CFGLGT"},

	{ CFGBGN,      "CFGBGN" },
	{ CFGEND,      "CFGEND" },
	{ SET_ID,      "SET_ID" },
	{ PING,        "PING" },
	{ RESET,       "RESET" },
	{ GETCFG,      "GETCFG" },
	{ GETDVC,      "GETDVC" },
	{ FLASH_REQ,   "FLASH_REQ  " },
	{ FLASH_DATA,  "FLASH_DATA " },
	{ FLASH_CHECK, "FLASH_CHECK" },
	{ QRYBUF,      "QRYBUF" },
	{ QRYERR,      "QRYERR" },
	{ GETVER,      "GETVER" },
	{ SENSOR,      "SENSOR" }
};

struct string_map result_map[] =
{
	{ MSG_OK,                 "MSG_OK" },
	{ MSG_QUEUE_FULL,         "MSG_QUEUE_FULL" },
	{ MSG_UNKNOWN_CMD,        "MSG_UNKNOWN_CMD" },
	{ MSG_PENDING,            "MSG_PENDING" },
	{ MSG_IGNORED,            "MSG_IGNORED" },
	{ MSG_QUEUED,             "MSG_QUEUED" },
	{ MSG_NOT_CONFIGURED_YET, "MSG_NOT_CONFIGURED_YET" },
	{ MSG_NO_UNITNO_DEFINED,  "MSG_NO_UNITNO_DEFINED" },
	{ MSG_UNITTYPE_WRONG,     "MSG_UNITTYPE_WRONG" },
	{ MSG_RESET_PENDING,      "MSG_RESET_PENDING" },
	{ MSG_UNITNO_MISSING,     "MSG_UNITNO_MISSING" },
	{ MSG_UNIT_NOT_FOUND,     "MSG_UNIT_NOT_FOUND" },
	{ MSG_NOT_IN_CONFIG_MODE, "MSG_NOT_IN_CONFIG_MODE" },
	{ MSG_BOOTED,             "MSG_BOOTED" },
	{ MSG_ID_NOT_CHANGED,     "MSG_ID_NOT_CHANGED" },
	{ MSG_CHECKSUM_ERROR,     "MSG_CHECKSUM_ERROR" },
	{ MSG_INFO,               "MSG_INFO" },
	{ MSG_ID_CHANGE_DISABLED, "MSG_ID_CHANGE_DISABLED" },
	{ MSG_HARDWARE_MISMATCH,  "MSG_HARDWARE_MISMATCH" },
	{ MSG_SWITCH_FAILED,      "MSG_SWITCH_FAILED" },
	{ MSG_CONFIG_BUFFER_FULL, "MSG_CONFIG_BUFFER_FULL" }
};

struct string_map signal_map[] =
{
	{ SIGNAL_OFF,  "SIGNAL_OFF" },
	{ SIGNAL_HP0,  "SIGNAL_HP0" },
	{ SIGNAL_HP1,  "SIGNAL_HP1" },
	{ SIGNAL_HP2,  "SIGNAL_HP2" },
	{ SIGNAL_VR0,  "SIGNAL_VR0" },
	{ SIGNAL_VR1,  "SIGNAL_VR1" },
	{ SIGNAL_VR2,  "SIGNAL_VR2" },
	{ SIGNAL_SH0,  "SIGNAL_SH0" },
	{ SIGNAL_SH1,  "SIGNAL_SH1" },
	{ SIGNAL_TST,  "SIGNAL_TST" }
};

static unsigned int received = 0;

static char* find_cmd_text(uint8_t code)
{
	int i = 0;
	int max = sizeof(cmd_map) / sizeof(struct string_map);
	
	for (i = 0;i < max;i++)
	{
		if (cmd_map[i].code == code)
		{
			return cmd_map[i].text;
		}
	}
	return NULL;
}

static char* find_result_text(uint8_t code)
{
	int i = 0;
	int max = sizeof(result_map) / sizeof(struct string_map);
	
	for (i = 0;i < max;i++)
	{
		if (result_map[i].code == code)
		{
			return result_map[i].text;
		}
	}
	return NULL;
}

static char* find_signal_text(uint8_t code)
{
	int i = 0;
	int max = sizeof(signal_map) / sizeof(struct string_map);
	
	for (i = 0;i < max;i++)
	{
		if (signal_map[i].code == code)
		{
			return signal_map[i].text;
		}
	}
	return NULL;
}

void dump_can_msg(CAN_message *msg, unsigned char sum, const char *comment)
{
	struct timeb now;
	int          i;

	ftime(&now);
	printf("%ld.%03d # ID=%04x:%04x len=%d stat=%02x",
		now.time, now.millitm,
		msg->sid, msg->eid, msg->length, msg->status);
	for (i = 0;i < msg->length; i++)
	{
		printf(" 0x%02x", msg->data[i]);
	}
	printf(" # 0x%02x %s\n", sum, comment != NULL ? comment : "");
	fflush(stdout);
}

void dump_mrw_msg(CAN_message *msg, uint8_t checksum, const char *comment)
{
	uint8_t  cmd = msg->data[0];
	char    *cmd_text = find_cmd_text(cmd & CMD_MASK);

	if (cmd_text != NULL)
	{
		struct timeb now;
		int          i;

		ftime(&now);
		if (cmd & MSG_RESULT)
		{
			char   *res_text = find_result_text(msg->data[1]);

			printf("%ld.%03d # ID=%04x:%04x len=%d stat=%02x # %-12.12s %-20.20s %04x:%02x%02x",
				now.time, now.millitm,
				msg->sid, msg->eid, msg->length, msg->status,
				cmd_text,
				res_text != NULL ? res_text : "<unknown>",
				msg->eid,
				msg->data[IDX_INFO_START - 1], msg->data[IDX_INFO_START - 2]);
			for (i = IDX_INFO_START;i < msg->length; i++)
			{
				printf(" 0x%02x", msg->data[i]);
			}
			printf(" # 0x%02x %5d %s\n", checksum, ++received, comment != NULL ? comment : "");
		}
		else
		{
			printf("%ld.%03d # ID=%04x:%04x len=%d stat=%02x # %s",
				now.time, now.millitm,
				msg->sid, msg->eid, msg->length, msg->status, cmd_text);
			if (cmd == SETSGN)
			{
				char *sig_text = find_signal_text(msg->data[1]);
	
				printf(" %s", sig_text != NULL ? sig_text : "???");
			}
			else if ((cmd == FLASH_DATA) || (cmd == FLASH_CHECK))
			{
				printf(" $%02x%02x%02x", msg->data[3], msg->data[2], msg->data[1]);
				for (i = 4;i < msg->length; i++)
				{
					printf(" 0x%02x", msg->data[i]);
				}
			}
			else
			{
				for (i = 1;i < msg->length; i++)
				{
					printf(" 0x%02x", msg->data[i]);
				}
			}
			printf(" # 0x%02x %s\n", checksum, comment != NULL ? comment : "");
		}
		fflush(stdout);
	}
	else
	{
		dump_can_msg(msg, checksum, comment);
	}
}
