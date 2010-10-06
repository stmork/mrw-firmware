/*
**
**	$Filename:	mrw.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
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
#include "light_profile.h"

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

#define DONT_USE_SID_IN_RESULT

// command definitions
#define SETLFT	(unsigned char)(CAT_SWITCH|0x01)
#define SETRGT	(unsigned char)(CAT_SWITCH|0x02)
#define GETDIR	(unsigned char)(CAT_SWITCH|0x03)

#define SETRON	(unsigned char)(CAT_RAIL|0x01)
#define SETROF	(unsigned char)(CAT_RAIL|0x02)
#define GETRBS	(unsigned char)(CAT_RAIL|0x03)

#define SETSGN	(unsigned char)(CAT_SIGNAL|0x01)

#define CFGSWN	(unsigned char)(CAT_CONFIG|0x01)
#define CFGSWO	(unsigned char)(CAT_CONFIG|0x02)
#define CFGRAI	(unsigned char)(CAT_CONFIG|0x03)
#define CFGPF2	(unsigned char)(CAT_CONFIG|0x04)
#define CFGPF3	(unsigned char)(CAT_CONFIG|0x05)
#define CFGMF2	(unsigned char)(CAT_CONFIG|0x06)
#define CFGMF3	(unsigned char)(CAT_CONFIG|0x07)
#define CFGPL2	(unsigned char)(CAT_CONFIG|0x08)
#define CFGPL3	(unsigned char)(CAT_CONFIG|0x09)
#define CFGSL2	(unsigned char)(CAT_CONFIG|0x0a)
#define CFGML2	(unsigned char)(CAT_CONFIG|0x0b)
#define CFGML3	(unsigned char)(CAT_CONFIG|0x0c)
#define CFGML4	(unsigned char)(CAT_CONFIG|0x0d)
#define CFGLGT  (unsigned char)(CAT_CONFIG|0x0e)

#define CFGBGN	    (unsigned char)(CAT_EXT|0x01)
#define CFGEND	    (unsigned char)(CAT_EXT|0x02)
#define SET_ID      (unsigned char)(CAT_EXT|0x03)
#define PING		(unsigned char)(CAT_EXT|0x04)
#define RESET		(unsigned char)(CAT_EXT|0x05)
#define GETCFG		(unsigned char)(CAT_EXT|0x06)
#define GETDVC		(unsigned char)(CAT_EXT|0x07)
#define FLASH_REQ	(unsigned char)(CAT_EXT|0x08)
#define FLASH_DATA	(unsigned char)(CAT_EXT|0x09)
#define FLASH_CHECK	(unsigned char)(CAT_EXT|0x0a)
#define QRYBUF      (unsigned char)(CAT_EXT|0x0b)
#define QRYERR      (unsigned char)(CAT_EXT|0x0c)
#define GETVER      (unsigned char)(CAT_EXT|0x0d)
#define SENSOR      (unsigned char)(CAT_EXT|0x0e)

// Signal types
#define SIGNAL_OFF (unsigned char)0
#define SIGNAL_HP0 (unsigned char)1
#define SIGNAL_HP1 (unsigned char)2
#define SIGNAL_HP2 (unsigned char)3
#define SIGNAL_VR0 (unsigned char)4
#define SIGNAL_VR1 (unsigned char)5
#define SIGNAL_VR2 (unsigned char)6
#define SIGNAL_SH0 (unsigned char)7
#define SIGNAL_SH1 (unsigned char)8
#define SIGNAL_TST (unsigned char)9

// Switch states
#define SWITCH_STATE_LEFT  (unsigned char)1
#define SWITCH_STATE_RIGHT (unsigned char)2

// Sensor types
#define SENSOR_LIGHT       (unsigned char)1
#define SENSOR_TEMP        (unsigned char)2

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
#define TYPE_LIGHT        14

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

struct mrw_light
{
	uint8_t                     threshold;
	uint8_t                     lightness;
	uint8_t                     type;
	uint8_t                     counter;
	uint8_t                     quotient;
	uint8_t                     dimm;
	uint8_t                     nom;
	uint8_t                     denom;
	mrw_connection              pin;
	const struct light_profile *profile;
};


typedef union
{
	struct mrw_switch u_switch;
	struct mrw_form2  u_form2;
	struct mrw_form3  u_form3;
	struct mrw_rail   u_rail;
	struct mrw_signal u_signal;
	struct mrw_light  u_light;
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
