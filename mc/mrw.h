/*
**
**	$Filename:	mrw.h $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: mrw.h 931 2010-04-14 08:39:15Z smork $
**
**	Modellrailway micro controller
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

#ifndef MRW_H
#define MRW_H

#include <stdint.h>
#include "bit.h"

#define FIRMWARE_TYPE_CAN_NODE    0
#define FIRMWARE_TYPE_CAN_GATEWAY 1

#define CAT_SWITCH      0x00
#define CAT_RAIL        0x10
#define CAT_SIGNAL      0x20
#define CAT_CONFIG      0x30
#define CAT_EXT         0x40
#define CAT_MASK        0x70

#define CMD_NEED_UNITNO 0x40
#define MSG_RESULT      0x80

#define CMD_MASK        0x7f

#define CMD_ILLEGAL        0

// command definitions
#define SETLFT	(CAT_SWITCH|0x01)
#define SETRGT	(CAT_SWITCH|0x02)
#define GETDIR	(CAT_SWITCH|0x03)

#define SETRON	(CAT_RAIL|0x01)
#define SETROF	(CAT_RAIL|0x02)
#define GETRBS	(CAT_RAIL|0x03)

#define SETSGN	(CAT_SIGNAL|0x01)

#define CFGSWN	(CAT_CONFIG|0x01)
#define CFGSWO	(CAT_CONFIG|0x02)
#define CFGRAI	(CAT_CONFIG|0x03)
#define CFGPF2	(CAT_CONFIG|0x04)
#define CFGPF3	(CAT_CONFIG|0x05)
#define CFGMF2	(CAT_CONFIG|0x06)
#define CFGMF3	(CAT_CONFIG|0x07)
#define CFGPL2	(CAT_CONFIG|0x08)
#define CFGPL3	(CAT_CONFIG|0x09)
#define CFGSL2	(CAT_CONFIG|0x0a)
#define CFGML2	(CAT_CONFIG|0x0b)
#define CFGML3	(CAT_CONFIG|0x0c)
#define CFGML4	(CAT_CONFIG|0x0d)

#define CFGBGN	    (CAT_EXT|0x01)
#define CFGEND	    (CAT_EXT|0x02)
#define SET_ID      (CAT_EXT|0x03)
#define PING		(CAT_EXT|0x04)
#define RESET		(CAT_EXT|0x05)
#define GETCFG		(CAT_EXT|0x06)
#define GETDVC		(CAT_EXT|0x07)
#define FLASH_REQ	(CAT_EXT|0x08)
#define FLASH_DATA	(CAT_EXT|0x09)
#define FLASH_CHECK	(CAT_EXT|0x0a)
#define QRYBUF      (CAT_EXT|0x0b)
#define QRYERR      (CAT_EXT|0x0c)
#define GETVER      (CAT_EXT|0x0d)

// Signal types
#define SIGNAL_OFF 0
#define SIGNAL_HP0 1
#define SIGNAL_HP1 2
#define SIGNAL_HP2 3
#define SIGNAL_VR0 4
#define SIGNAL_VR1 5
#define SIGNAL_VR2 6
#define SIGNAL_SH0 7
#define SIGNAL_SH1 8
#define SIGNAL_TST 9

// Switch states
#define SWITCH_STATE_LEFT  1
#define SWITCH_STATE_RIGHT 2

// result definitions
#define NO_RESULT                -1 // not really sent
#define MSG_OK                    0
#define MSG_QUEUE_FULL            1
#define MSG_UNKNOWN_CMD           2
#define MSG_PENDING               3
#define MSG_IGNORED               4
#define MSG_QUEUED                5
#define MSG_NOT_CONFIGURED_YET    6
#define MSG_NO_UNITNO_DEFINED     7
#define MSG_UNITTYPE_WRONG        8
#define MSG_RESET_PENDING         9
#define MSG_UNITNO_MISSING       10
#define MSG_UNIT_NOT_FOUND       11
#define MSG_NOT_IN_CONFIG_MODE   12
#define MSG_BOOTED               13
#define MSG_ID_NOT_CHANGED       14
#define MSG_CHECKSUM_ERROR       15
#define MSG_INFO                 16
#define MSG_ID_CHANGE_DISABLED   17
#define MSG_HARDWARE_MISMATCH    18
#define MSG_SWITCH_FAILED        19

#define TYPE_SWITCH_OLD    0
#define TYPE_SWITCH_NEW    1
#define TYPE_SIGNAL_PF2    2
#define TYPE_SIGNAL_MF2    3
#define TYPE_SIGNAL_SF2    4
#define TYPE_SIGNAL_PF3    5
#define TYPE_SIGNAL_MF3    6
#define TYPE_RAIL          7
#define TYPE_SIGNAL_PL2    8
#define TYPE_SIGNAL_ML2    9
#define TYPE_SIGNAL_SL2   10
#define TYPE_SIGNAL_PL3   11
#define TYPE_SIGNAL_ML3   12
#define TYPE_SIGNAL_ML4   13

#define SWITCH_DEPRELL 3

struct mrw_switch
{
	mrw_connection left;
	mrw_input      left_input;
	mrw_connection right;
	mrw_input      right_input;
};

struct mrw_form2
{
	mrw_connection inductor[2];
};

struct mrw_form3
{
	mrw_connection inductor[3];
};

struct mrw_rail
{
	mrw_connection power;
	mrw_input      occupation;  // rail occupation message
};

struct mrw_signal
{
	uint8_t        count;
	uint8_t        img;
	uint8_t        byte[5];
	uint8_t        bit[5];
};


typedef union
{
	struct mrw_switch u_switch;
	struct mrw_form2  u_form2;
	struct mrw_form3  u_form3;
	struct mrw_rail   u_rail;
	struct mrw_signal u_signal;
} mrw_unit;

typedef struct
{
	uint16_t unit_no;
	uint8_t  unit_type;
	mrw_unit unit;
} mrw_device;


typedef struct
{
	mrw_device *device;
	uint8_t     cmd;
	uint8_t     code;
} command;

#endif
