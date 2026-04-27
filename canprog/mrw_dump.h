/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Dump clear text CAN frames.
**
*/

#pragma once

#ifndef MRW_DUMP_H
#define MRW_DUMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mrw.h"
#include "mcp2515.h"

extern void dump_can_msg(CAN_message * msg, unsigned char checksum, const char * comment);
extern void dump_mrw_msg(CAN_message * msg, uint8_t       checksum, const char * comment);

#ifdef __cplusplus
};
#endif

#endif
