/*
**
**	$Filename:	pwm.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	PWM soft handler
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

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "pwm.h"

/* Zählertabelle */
static const uint8_t table_nom[PWM_TABLE_SIZE] PROGMEM =
{
	0x00, 0x01, 0x01, 0x03, 0x01, 0x05, 0x03, 0x07, 0x01, 0x09, 0x05, 0x0b, 0x03, 0x0d, 0x07, 0x0f,
	0x01, 0x11, 0x09, 0x13, 0x05, 0x15, 0x0b, 0x17, 0x03, 0x19, 0x0d, 0x1b, 0x07, 0x1d, 0x0f, 0x1
};

/* Nennertabelle*/
static const uint8_t table_denom[PWM_TABLE_SIZE] PROGMEM =
{
	0x20, 0x20, 0x10, 0x20, 0x08, 0x20, 0x10, 0x20, 0x04, 0x20, 0x10, 0x20, 0x08, 0x20, 0x10, 0x20,
	0x02, 0x20, 0x10, 0x20, 0x08, 0x20, 0x10, 0x20, 0x04, 0x20, 0x10, 0x20, 0x08, 0x20, 0x10, 0x1
};

/*
 * Hier wird aus einer Tabelle Zähler und Nenner für die Helligkeit ausgelesen. Wichtig hier
 * ist, dass die Werte aus dem Flash geholt werden (const/PROGMEM), um RAM zu sparen.
 */
void set_dimm(struct mrw_light *dvc, uint8_t value)
{
	value >>= PWM_SHIFT;

	if (value == 0)
	{
		/* Einmal ausschalten und gar nicht erst rumdimmen! */
		dvc->dimm = 0;
		clr_pin(&dvc->pin);
	}
	else if (dvc->dimm != value)
	{
		/* Werte neu initialisieren */
		dvc->dimm     = value;
		dvc->quotient = 0;
		dvc->nom      = pgm_read_byte(&table_nom[value]);
		dvc->denom    = pgm_read_byte(&table_denom[value]);

		if (value == PWM_MAX)
		{
			set_pin(&dvc->pin);
		}
	}
}

/*
 * Das hier ist die Soft-PWM-Routine. Sie kann acht Kanäle gleichzeitig unabhängig
 * voneinander bearbeiten. Der Trick, hier ohne langsame Division auszukommen ist,
 * dass so ähnlich wie beim Linienzeichnen (Gfx!!), die Steigung durch Annäherung
 * nachgehalten wird. Da die Steigung immer zwischen [0.0, 1.0] ist, ist die Steigung
 * immer flach, was den Algorithmus vereinfacht. Diese Routine dürfte nicht mehr
 * als 300 Takte brauchen.
 */
void handle_pwm(struct mrw_light *dvc)
{
	/* Dimmen */
	if (dvc->quotient < dvc->nom)
	{
		dvc->quotient += dvc->denom;
		set_pin(&dvc->pin);
	}
	else
	{
		clr_pin(&dvc->pin);
	}
	dvc->quotient -= dvc->nom;
}
