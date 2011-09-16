/*
**
**  $Revision$
**  $Date$
**  $Author$
**  $Id$
**
**	Logger
**
**  Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**  This program and the accompanying materials are made available under the
**  terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**  which accompanies this distribution.
**
**  The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#include <sys/timeb.h>
#include "Log.h"
#include <algorithm>

using namespace std;

#ifdef USE_SID_IN_RESULT
#define IDX_INFO_START 6
#else
#define IDX_INFO_START 4
#endif

Log logger;

struct string_map
{
	const uint8_t  code;
	const char    *text;
};

struct string_map command_values[] =
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

struct string_map result_values[] =
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

struct string_map signal_values[] =
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

const char* Log::find_cmd_text(uint8_t code)
{
	mrw_constant_map::iterator it = command_map.find(code);

	return it != command_map.end() ? it->second : NULL;
}

const char *Log::find_result_text(uint8_t code)
{
	mrw_constant_map::iterator it = result_map.find(code);

	return it != result_map.end() ? it->second : NULL;
}

const char *Log::find_signal_text(uint8_t code)
{
	mrw_constant_map::iterator it = signal_map.find(code);

	return it != signal_map.end() ? it->second : NULL;
}

Log::Log()
{
	int i;

	for (i = 0; i < sizeof(command_values)/sizeof(struct string_map);i++)
	{
		command_map.insert(make_pair(command_values[i].code, command_values[i].text));
	}
	for (i = 0; i < sizeof(result_values)/sizeof(struct string_map);i++)
	{
		result_map.insert(make_pair(result_values[i].code, result_values[i].text));
	}
	for (i = 0; i < sizeof(signal_values)/sizeof(struct string_map);i++)
	{
		signal_map.insert(make_pair(signal_values[i].code, signal_values[i].text));
	}
}

void Log::Dump(const CAN_message *msg, uint8_t checksum, const char *comment)
{
	struct timeb  now;
	::Lock        lock(*this);
	uint8_t       cmd = msg->data[0];
	const char   *cmd_text = find_cmd_text(cmd & CMD_MASK);

	ftime(&now);
	if (cmd_text != NULL)
	{
		int     i;

		if (cmd & MSG_RESULT)
		{
			const char *res_text = find_result_text(msg->data[1]);

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
				const char *sig_text = find_signal_text(msg->data[1]);
	
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
	}
	else
	{
		int    i;

		printf("%ld.%03d # ID=%04x:%04x len=%d stat=%02x",
			now.time, now.millitm,
			msg->sid, msg->eid, msg->length, msg->status);
		for (i = 0;i < msg->length; i++)
		{
			printf(" 0x%02x", msg->data[i]);
		}
		printf(" # 0x%02x %s\n", checksum, comment != NULL ? comment : "");
	}
	fflush(stdout);
}

void Log::Info(const char *message)
{
	::Lock lock(*this);

	puts(message);
	fflush(stdout);
}

void Log::Error(const char *message)
{
	::Lock lock(*this);

	perror(message);
	fflush(stderr);
}
