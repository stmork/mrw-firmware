/*
**
**	$Filename:	config.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Verwaltung der Konfigurationsdaten
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

#ifndef CONFIG_H
#define CONFIG_H

#include "eeprom.h"

extern struct eeprom_area config;

extern mrw_device *find_device(uint16_t unit_no);
extern void        config_sort(void);
extern void        init_light_signal(mrw_device *dvc, uint8_t *buffer, uint8_t count, uint8_t type);

#endif
