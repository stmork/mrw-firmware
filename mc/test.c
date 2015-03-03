/*
**
**	$Filename:	test.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Firmware for testing basic functions.
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

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "mcp2515.h"
#include "bit.h"
#include "timer.h"
#include "sleep.h"
#include "eeprom.h"

#define NODE_OFF       0
#define NODE_READY     MCP_LED_GREEN
#define NODE_OVERFLOW (MCP_LED_GREEN|MCP_LED_YELLOW)

static mrw_connection LED1;
static mrw_connection LED2;
static mrw_connection LED3;
static mrw_connection LED4;
static mrw_input      taster;

static uint8_t        counter = 0;

static uint8_t        mcp2515_register_map[256];

static struct eeprom_area config;

/**
 * Diese Methode zeigt die untersten beiden Bits an zwei
 * Pins in Form von LEDs an.
 */
static void state(uint8_t state)
{
	if (bit_is_set(state, 0))
	{
		set_pin(&LED1);
	}
	else
	{
		clr_pin(&LED1);
	}

	if (bit_is_set(state, 1))
	{
		set_pin(&LED2);
	}
	else
	{
		clr_pin(&LED2);
	}

	if (bit_is_set(state, 2))
	{
		set_pin(&LED3);
	}
	else
	{
		clr_pin(&LED3);
	}

	if (bit_is_set(state, 3))
	{
		set_pin(&LED4);
	}
	else
	{
		clr_pin(&LED4);
	}
}

static void pong(void)
{
	CAN_message msg;
		
	msg.length  = 6;
	msg.status  = 0;
	msg.sid     = GATEWAY_SID;
	msg.eid     = 0;
	msg.data[0] = PING | MSG_RESULT;
	msg.data[1] = MSG_OK;
	msg.data[2] = config.id & 0xff;
	msg.data[3] = config.id >> 8;
	msg.data[4] = 0;
	msg.data[5] = 0;

	can_send_msg(&msg);
}

/**
 * Hier findet die eigentliche Verarbeitung statt.
 * Die Verarbeitung ist hier nur das Hochzählen samt Anzeigen an LEDs.
 */
static void process_can_message(void)
{
	CAN_message msg;
	
	/* Es werden alle empfangenen CAN-Messages verarbeitet. */
	while (can_get_msg(&msg) >= 0)
	{
//		state(msg.data[0]);
		state(++counter);
		if ((msg.length >= 1) && (msg.data[0] == PING))
		{
			pong();
		}
		else if ((msg.length >= 1) && (msg.data[0] == RESET))
		{
			cli();
			mcp2515_write_rx_output_pins(NODE_OFF);
			wdt_enable(WDTO_15MS);
			while (1);
		}
	}
}

/**
 * Interrupt Service Routine externer Interrupt 2. Hier wird eine
 * CAN-Message empfangen und zur Weiterverarbeitung vorbereitet.
 */
ISR(INT2_vect)
{
	process_can_message();
}

/**
 * Hier werden zwei LEDs und ein Taster initialisiert. Es werden die
 * API Methoden verwendet, die auch die MRW Firmware benutzt.
 */
static void port_init(void)
{
	config_connection(&LED1, 0);
	config_connection(&LED2, 1);
	config_connection(&LED3, 2);
	config_connection(&LED4, 3);
	config_connection(&taster.connection, 23);

	out_pin(&LED1);
	out_pin(&LED2);
	out_pin(&LED3);
	out_pin(&LED4);

	init_input(&taster, 1);
	state(2);
}

/**
 * Interrupt Service Routine Overflow Timer 2. Hier wird im 64 Hz Takt
 * der Zustand des Tasters überprüft. Die Zustandüberprüfung entprellt
 * gleichzeitig den Taster. Da der Input Active Low ist, entspricht eine
 * fallende Flanke dem Drücken des Tasters. In diesem Fall wird eine
 * CAN-Message versendet.
 */
ISR(TIMER2_OVF_vect)
{
#if 0
	state(++counter);
#else
	// Taster ist active low, daher ist fallende Flanke drücken.
	if (test_state_change(&taster) == PIN_FALLING_EDGE)
	{
		pong();
		counter++;
		state(counter);
	}
#endif
}

int main(void)
{
	/* Konfiguration aus EEPROM auslesen */
	uint8_t is_configured = read_eeprom_config(&config);

	/* Wenn keine Konfiguration vorliegt, wird eine ID gebrannt. */
	if (!is_configured)
	{
		config.id    = 1;
		config.count = 0;
		save_eeprom_id(&config);
	}

	/* Initialisieren und ab dafür! */
	port_init();
	timer2_init();

	mcp2515_init(config.id, is_configured, MCP2515_MULTI_TX_BUFFER);
	mcp2515_dump_register(mcp2515_register_map);

	mcp2515_write_rx_output_pins(NODE_READY);
	sei();
	while(1)
	{
		cli();
		sleep();

		MCP2515_error_status status;

		/*
		 * Wenn Fehler/Warn Status vorhanden ist, dann muss
		 * darauf reagiert werden. Am Wahrscheinlichsten sind
		 * Overflows bei zu hoher Message-Rate oder Error im
		 * Fall von Leitungsproblemen.
		 */
		if (mcp2515_read_error_status(&status))
		{
			/*
			 * Hier werden evtl. nicht verabeitete CAN-Messages aus
			 * ihren Empfangspuffern geholt. Das muss bei gesperrtem
			 * Interrupt erfolgen.
			 */
			cli();
			process_can_message();
			sei();

			mcp2515_dump_register(mcp2515_register_map);
			state(status.eflg >> 6);
			mcp2515_reset_overflow(status.eflg);
		}
	}
}
