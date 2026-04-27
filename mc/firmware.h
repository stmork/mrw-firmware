/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Konstanten der CAN-Knoten Firmware.
**
*/

#ifndef FIRMWARE_H
#define FIRMWARE_H

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#define RING_SIZE      32
#define RING_MASK     (RING_SIZE - 1)

extern struct eeprom_area config;

#endif
