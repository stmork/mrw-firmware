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

#include <stdlib.h>

#include "config.h"
#include "tool.h"

// device configuration
struct eeprom_area config;

/**
 * Diese Routine sucht an Hand einer Unitnumber die Device-Konfiguration.
 */
mrw_device *find_device(uint16_t unit_no)
{
	mrw_device *dvc;
	uint8_t     min = 0;
	uint8_t     max = config.count;
	uint8_t     mid;

	while(min < max)
	{
		mid = (min + max) >> 1;
		dvc = &config.dvc[mid];
		
		if (dvc->unit_no < unit_no)
		{
			min = mid;
		}
		else if (dvc->unit_no > unit_no)
		{
			max = mid;
		}
		else
		{
			return dvc;
		}
	}

	return null;
}

static int compare_device(const void *A, const void *B)
{
	const mrw_device *a = A;
	const mrw_device *b = B;

	return a->unit_no - b->unit_no;
}

void config_sort(void)
{
	qsort(config.dvc, config.count, sizeof(mrw_device), compare_device);
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

	for (uint8_t i = 0;i < count;i++)
	{
		dvc->unit.u_signal.byte[i] = buffer[i] >> 3;
		dvc->unit.u_signal.bit[i]  = buffer[i] & 0x7;
	}
}
