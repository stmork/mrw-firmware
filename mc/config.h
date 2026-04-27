/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Verwaltung der Konfigurationsdaten
**
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "eeprom.h"

extern struct eeprom_area config;

extern mrw_device *find_device(uint16_t unit_no);
extern void        config_sort(void);
extern void        init_light_signal(mrw_device *dvc, uint8_t *buffer, uint8_t count, uint8_t type);

#endif
