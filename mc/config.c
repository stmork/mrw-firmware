/*
**
**	$Filename:	config.c $
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

#include "config.h"
#include "tool.h"

// device configuration
struct eeprom_area config;

/**
 * Diese Routine sucht an Hand einer Unitnumber die Device-Konfiguration.
 */
mrw_device *find_device(uint16_t unit_no)
{
	for (uint8_t i = 0;i < config.count;i++)
	{
		if (config.dvc[i].unit_no == unit_no)
		{
			return &config.dvc[i];
		}
	}
	return null;
}

/**
 * Hier werden alle Bit- und Byte-Positionen für die Lichtsignale berechnet.
 */
void init_light_signal(
	mrw_device *dvc,
	uint8_t    *buffer,
	uint8_t     count,
	uint8_t     type)
{
	dvc->unit_type = type;
	dvc->unit.u_signal.count = count;
	for (int i = 0;i < count;i++)
	{
		dvc->unit.u_signal.byte[i] = buffer[i] >> 3;
		dvc->unit.u_signal.bit[i]  = buffer[i] & 0x7;
	}
}

