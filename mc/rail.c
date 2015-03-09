/*
**
**	$Filename:	rail.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Modellrailway micro controller
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

#include "rail.h"
#include "bit.h"

/**
 * Diese Methode initialisiert einen Gleisabschnitt. Ein
 * Output Pin wird für das Fahrstromrelais verwendet, ein
 * Inout Pin wird für die Gleisbesetztmeldung gebraucht.
 */
void rail_init(struct mrw_rail *device)
{
	out_pin   (&device->power);
	init_input(&device->occupation, 1);
}

/**
 * Diese Methode schaltet den Fahrstrom auf den Gleisabschnitt.
 */
void rail_on(struct mrw_rail *device)
{
	set_pin(&device->power);
}

/**
 * Diese Methode schaltet den Fahrstrom auf dem Gleisabschnitt ab.
 */
void rail_off(struct mrw_rail *device)
{
	clr_pin(&device->power);
}

/**
 * Diese Methode ermittelt, ob es einen Zustandswechsel bei
 * der Gleisbesetztmeldung gegeben hat.
 */
uint8_t rail_state_changed(struct mrw_rail *device)
{
	return test_state_change(&device->occupation) != PIN_NO_CHANGE;
}

/**
 * Diese Methode meldet den aktuellen Gleisbesetztzustand zurück.
 */
uint8_t rail_occupied(struct mrw_rail *device)
{
	return !device->occupation.state;
}
