/*
**
**	$Filename:	eeprom.h $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	EEPROM handling
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

#ifndef EEPROM_H
#define EEPROM_H

#include <stddef.h>
#include "avr/eeprom.h"
#include "mrw.h"   

#define WAIT_EEPROM while(!eeprom_is_ready())
#define MAX_DEVICES 44

struct eeprom_area
{
	uint16_t     id;
	uint8_t      count;
	uint8_t      magic;
	mrw_device   dvc[MAX_DEVICES];
};

#define EEPROM_ADDR_ID           ((void *)offsetof(struct eeprom_area, id))
#define EEPROM_ADDR_DEVICE_COUNT ((void *)offsetof(struct eeprom_area, count))
#define EEPROM_ADDR_MAGIC        ((void *)offsetof(struct eeprom_area, magic))
#define EEPROM_ADDR_START_CONFIG ((void *)offsetof(struct eeprom_area, dvc))

#define CONFIG_MAGIC 0xa5

extern uint16_t read_eeprom_module_uid(void);
extern uint8_t  is_eeprom_configured(void);
extern uint8_t  read_eeprom_config(struct eeprom_area *config);
extern void     save_eeprom_config(struct eeprom_area *config);
extern void     save_eeprom_id    (struct eeprom_area *config);

#endif
