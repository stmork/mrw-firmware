/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway micro controller
**
*/

#ifndef TOOL_H
#define TOOL_H

#ifndef null
#define null 0
#endif

#ifndef FALSE
#define FALSE 0
#endif


#ifndef TRUE
#define TRUE 1
#endif

#ifndef NOP
#define NOP    { asm volatile ("nop"); }
#endif


#endif
