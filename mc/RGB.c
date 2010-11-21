/*
**
**	$Filename:	test_light.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Simple RGB diode light testing.
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

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdlib.h>
#include <avr/io.h>

#include "config.h"
#include "eeprom.h"
#include "mcp2515.h"
#include "sleep.h"

#define NODE_READY     MCP_LED_GREEN

#define B1 1
#define R  2
#define B2 4
#define G  8

static void init(void)
{
	DDRA  = 0xff;
	PORTA = 0xee;
	PINA  = 0;
}

int main(void)
{
	read_eeprom_config(&config);

	/* Takt initialisieren, so dass MCP2515 seine 16 MHz auch her gibt. */
	/* Ansonsten passiert hier heute per CAN-Bus nix ;-) */
	mcp2515_init(config.id, 1, MCP2515_SINGLE_TX_BUFFER);
	init();
	mcp2515_write_rx_output_pins(NODE_READY);

	do
	{
		/*
		 * Ein bißchen was für die Umwelt tun. Es fängt halt
		 * halt schon im Kleinen an ;-)
		 */
		sleep();
	}
	while(1);
}
