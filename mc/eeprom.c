/*
**
**	$Filename:	eeprom.c $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: eeprom.c 931 2010-04-14 08:39:15Z smork $
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

#include "eeprom.h"
#include "tool.h"
#include "mcp2515.h"
#include <avr/wdt.h>

/**
 * Diese Routine liest die Controller ID aus.
 */
uint16_t read_eeprom_module_uid(void)
{
	if (is_eeprom_configured())
	{
		WAIT_EEPROM;
		return eeprom_read_word(EEPROM_ADDR_ID);
	}
	return UNDEFINED_SID;
}

/**
 * Diese Routine prüft auf eine gültige Konfiguration. Dazu muss das
 * Magic Byte im EEPROM auf dem richtigen Wert stehen.
 */
uint8_t is_eeprom_configured(void)
{
	WAIT_EEPROM;
	return (eeprom_read_byte(EEPROM_ADDR_MAGIC) == CONFIG_MAGIC) ? TRUE : FALSE;
}

/**
 * Diese Routine liest die gesamte Konfiguration aus dem EEPROM und
 * kopiert sie ins RAM. Dabei wird berücksichtigt, ob die Konfiguration
 * gültig ist. Wenn nicht, sind Controller ID und Geräteanzahl auf 0 gesetzt.
 * Das Ergebnis entspricht dem von is_eeprom_configured().
 */
uint8_t read_eeprom_config(struct eeprom_area *config)
{
	// First check for Magic bit pattern to know if device was configured:
	uint8_t configured = is_eeprom_configured();

	if (configured)
	{
		WAIT_EEPROM;
		config->id = read_eeprom_module_uid();

		// Read device count
		WAIT_EEPROM;
		config->count = eeprom_read_byte(EEPROM_ADDR_DEVICE_COUNT);

		// copy config block from EEPROM to RAM
		WAIT_EEPROM;
		eeprom_read_block(config->dvc, EEPROM_ADDR_START_CONFIG,
					  sizeof(mrw_device) * config->count);

		config->magic = CONFIG_MAGIC;
	}
	else
	{
		config->count = 0;
		config->id    = 0;
		config->magic = 0;
	}
	return configured;
}

/**
 * Diese Routine speichert die aktuelle Konfiguration im EEPROM.
 * Das geschieht sicher, so dass ein Abbruch im Zweifelsfall einer
 * ungültigen Konfiguration entspricht bzw. keine Geräte konfiguriert
 * sind.
 */
void save_eeprom_config(struct eeprom_area *config)
{
	uint8_t to = WDTCR & 0x7;
	
	wdt_disable();
	WAIT_EEPROM;
	eeprom_write_byte(EEPROM_ADDR_DEVICE_COUNT, 0);

	// copy config block from EEPROM to RAM
	if (config->count > 0)
	{
		WAIT_EEPROM;
		eeprom_write_block(config->dvc, EEPROM_ADDR_START_CONFIG,
						   sizeof(mrw_device) * config->count);

		WAIT_EEPROM;
		eeprom_write_byte(EEPROM_ADDR_DEVICE_COUNT, config->count);
	}

	// now check for Magic bit pattern to know if device was configured:
	WAIT_EEPROM;
	eeprom_write_byte(EEPROM_ADDR_MAGIC, CONFIG_MAGIC);

	WAIT_EEPROM;
	wdt_enable(to);
}

/**
 * Diese Routine speichert eine neue Controller ID im EEPROM. Diese
 * Routine sollte nur aufgerufen werden, wenn sich die ID wirklich
 * ändert und auch die Gerätezahl auf 0 gesetzt wurde.
 */
void save_eeprom_id(struct eeprom_area *config)
{
	WAIT_EEPROM;
	eeprom_write_byte(EEPROM_ADDR_DEVICE_COUNT, config->count);

	WAIT_EEPROM;
	eeprom_write_word(EEPROM_ADDR_ID, config->id);

	// now check for Magic bit pattern to know if device was configured:
	WAIT_EEPROM;
	eeprom_write_byte(EEPROM_ADDR_MAGIC, CONFIG_MAGIC);

	WAIT_EEPROM;
}
