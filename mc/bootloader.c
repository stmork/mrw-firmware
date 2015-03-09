/*
**
**	$Filename:	bootloader.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	ATmega CAN bootloader
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
#define F_CPU 16000000L
#endif

#define no_FLASH_SIM

#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "mcp2515.h"
#include "eeprom.h"
#include "mrw.h"
#include "signature.h"

#define GET_WORD(ptr) ((((uint16_t)(ptr)[1]) << 8) | ((ptr)[0]))

#define PAGE_MASK   ((SPM_PAGESIZE) - 1)
#define PAGE_START  (0x1ffff ^ (PAGE_MASK))

#define MODE_FLASH    1
#define MODE_WAIT     2
#define MODE_APPSTART 3

uint8_t  buffer[SPM_PAGESIZE];
uint8_t  signature[4];
uint16_t page_count = 0;

typedef void (*app_ptr)(void);

static app_ptr main_entry = (app_ptr)0x0000;

/**
 * Hier wird mittels des watch dogs ein Reset ausgelöst. Der Watchdog wird
 * erst hier aktiviert und in eine Endlosschleife verzweigt.
 */
static void reset(void)
{
	mcp2515_write_rx_output_pins(0);
	wdt_enable(WDTO_15MS);
	while(1);
}

static void can_fill_message(
	CAN_message *msg,
	uint16_t     cid,
	uint8_t      code)
{
	msg->sid      = GATEWAY_SID;
	msg->eid      = cid;
	msg->status   = _BV(FRAME_EXT);			

	msg->length   = 0;
	msg->data[msg->length++] |= MSG_RESULT;
	msg->data[msg->length++]  = code;
#ifdef USE_SID_IN_RESULT
	msg->data[msg->length++] = cid & 0xff ;
	msg->data[msg->length++] = cid >> 8;
#endif
	msg->data[msg->length++] = 0;
	msg->data[msg->length++] = 0;
}

static void can_reply_message(
	CAN_message *msg,
	uint16_t     cid,
	uint8_t      code)
{
	can_fill_message(msg, cid, code);
	can_send_msg(msg);
}

static void can_reply_message_infos(
	CAN_message *msg,
	uint16_t     cid,
	uint8_t      code,
	uint8_t      info1,
	uint8_t      info2)
{
	can_fill_message(msg, cid, code);
	msg->data[msg->length++] = info1;
	msg->data[msg->length++] = info2;
	while (can_put_msg(msg) < 0)
	{
		_delay_ms(1);
	}
}

/**
 * Diese Methode bereitet einen kontrollierten Einstieg in die
 * eigentliche Applikation vor. Wenn es eine Applikation im
 * Flash ab Adresse $0000 liegt, sind da keine $ff drin.
 */
static void app_start(void)
{
	uint8_t start_ptr[4];
	uint8_t i, counter = 0;

	mcp2515_write_rx_output_pins(MODE_APPSTART);

	/*
	 * Hier wird geprüft, ob ein JMP-Befehl ab Adresse
	 * $0000 steht, der nicht auf die Adresse $ffff
	 * zeigt. Ein unprogrammiertes Flash enthält $ff.
	 */
	for(i = 0;i < 4;i++)
	{
		start_ptr[i] = pgm_read_byte_near(i);
	}
	if ((start_ptr[0] == 0x0c) &&
	    (start_ptr[1] == 0x94) &&
	   ((start_ptr[2] != 0xff) ||
	    (start_ptr[3] != 0xff)))
	{
		mcp2515_write_rx_output_pins(0);
		main_entry();
	}

	/*
	 * Bei einem unprogrammierten Flash wird auf ein RESET auf dem
	 * CAN-Bus gewartet.
	 */
	do
	{
		CAN_message msg;

		mcp2515_write_rx_output_pins(counter & 0x80 ? MCP_LED_GREEN|MCP_LED_YELLOW : MCP_LED_GREEN);
		if(can_get_msg(&msg) >= 0)
		{
			if (msg.data[0] == RESET)
			{
				reset();
			}
		}
		_delay_ms(1);
		counter++;
	}
	while(1);
}

/**
 * Diese Methode flasht eine einzelne Flash Page. Wie allgemein
 * üblich, wird zuerst die betroffene Page geloöscht und danach
 * beschrieben.
 */
static void boot_program_page (uint16_t address, uint8_t *buf)
{
	uint32_t page = address & PAGE_START;
#ifdef FLASH_SIM
	_delay_ms(5);
#else
	uint16_t i;

	eeprom_busy_wait ();
	boot_page_erase (page);
	boot_spm_busy_wait (); // Wait until the memory is erased.

	for (i = 0;i < SPM_PAGESIZE; i += 2)
	{
		// Set up little-endian word.
		uint16_t w = *buf++;
		w += (*buf++) << 8;
		boot_page_fill (page + i, w);
	}
	boot_page_write (page); // Store buffer in flash page.
	boot_spm_busy_wait(); // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.
	boot_rww_enable ();
#endif

	page_count++;
}

/**
 * Diese Methode bildet die Prüfsumme eines Flash-Speicherbereiches.
 */
static uint8_t sum_flash(uint16_t start, uint16_t end)
{
	uint8_t sum = 0;

	while (start < end)
	{
		sum += pgm_read_byte(start);
		start++;
	}
	return sum;
}

/**
 * Diese Methode vergleicht den Controllertyp auf
 * Basis der FLASH_REQ-Befehls. Nur wenn der
 * Controllertyp zum eigenen Controller passt,
 * wird ein Flash-Vorgang eingeleitet.
 */
static uint8_t check_controller_type(CAN_message *msg)
{
	if (msg->length >= 5)
	{
		int i;
		for (i = 0;i < 4;i++)
		{
			if (signature[i] != msg->data[i+1])
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/**
 * Dies hier ist die Hauptschleife für den Flash-Vorgang.
 */
static void flash(uint16_t cid)
{
	CAN_message msg;
	uint8_t     checksum = 0;
	uint16_t    address;
	uint16_t    idx = 0;

	signature[0] = mcp2515_read_tx_input_pins() & CAN_HARDWARE_SIG;
#if defined( __AVR_ATmega32__)
	signature[1] = 0x1e;
	signature[2] = 0x95;
	signature[3] = 0x02;
#else
#	error "No valid MCU type defined."
#endif

	do
	{
		if (can_get_msg(&msg) >= 0)
		{
			uint8_t cmd = msg.data[0];

			switch(cmd)
			{
			case RESET:
				can_reply_message(&msg, cid, MSG_OK);
				reset();
				break;

			case PING:
				can_reply_message(&msg, cid, MSG_OK);
				break;

			case FLASH_REQ:
				// Firmware type and controller type id must match!
				if (check_controller_type(&msg) == 0)
				{
					can_reply_message_infos(&msg, cid,
						MSG_HARDWARE_MISMATCH,
						msg.data[1], signature[0]);

					// Return to app if not
					return;
				}
				break;
				
			case FLASH_DATA:
				address = GET_WORD(&msg.data[1]);
				idx     = address & PAGE_MASK;
				if (msg.length >= 6)
				{
					buffer[idx++] = msg.data[4];
					buffer[idx++] = msg.data[5];
					checksum += msg.data[4];
					checksum += msg.data[5];

					if (msg.length == 8)
					{
						buffer[idx++] = msg.data[6];
						buffer[idx++] = msg.data[7];
						checksum += msg.data[6];
						checksum += msg.data[7];
					}
				}
				uint16_t offset = PAGE_MASK;
				if ((idx & offset) == 0)
				{
					boot_program_page(address, buffer);
				}
				break;

			case FLASH_CHECK:
				if (msg.length == 5)
				{
					address = GET_WORD(&msg.data[1]);
					uint8_t to_check = msg.data[4];

					if (checksum == to_check)
					{
						boot_program_page(address, buffer);
					}

					uint8_t flashsum = sum_flash(0, address);
					uint8_t success  = (checksum == to_check) && (flashsum == to_check);

					can_reply_message_infos(&msg, cid,
						success ?
						MSG_OK : MSG_CHECKSUM_ERROR,
						checksum, flashsum);

					/*
					 * Hier wird max. 150 ms darauf gewartet, dass die Sendepuffer komplett
					 * frei werden. Wenn die 150ms abgelaufen sind, wird die Hauptapplikation
					 * trotzdem gestartet.
					 */
					for (uint8_t cnt = 0;
						(mcp2515_read_status() & (TX_STAT_TXB0|TX_STAT_TXB1|TX_STAT_TXB2)) &&
						(cnt < 150);
						cnt++)
					{
						_delay_ms(1);
					}

					// return to app if OK.
					return;
				}
				break;

			default:
				if (page_count == 0)
				{
					// return to App
					can_reply_message(&msg, cid, MSG_IGNORED);
				}
			}
		}
	}
	while (1);
}

/**
 * Der Bootloader hat die Aufgabe, in den ersten zwei Sekunden nach
 * dem Starten ggf. eine neue Firmware per CAN-Bus entgegenzunehmen
 * und zu flashen. In dieser Zeit muss also ein FLASH_REQ-Befehl
 * mit dem passenden Controllertyp empfangen werden.
 */
int main(void)
{
	wdt_disable();
	int16_t counter;

#if defined( __AVR_ATmega32__)
//	boot_lock_bits_set_safe(0x10); // safe myself.
#endif
	uint16_t cid = read_eeprom_module_uid();
	mcp2515_init(cid, is_eeprom_configured(), MCP2515_SINGLE_TX_BUFFER);
	cli();

	// Wait max. 2 seconds.
	for (counter = 2000; counter >= 0;counter--)
	{
		CAN_message msg;
		int8_t      can_buffer;

		_delay_ms(1);
		mcp2515_write_rx_output_pins(counter & 0x80 ? MCP_LED_GREEN : 0);
		can_buffer = can_get_msg(&msg);
		if (can_buffer >= 0)
		{
			uint8_t cmd = msg.data[0];
			if (cmd == PING)
			{
				can_reply_message(&msg, cid, MSG_OK);
			}
			else if (cmd == FLASH_REQ)
			{
				mcp2515_write_rx_output_pins(1);
				can_reply_message(&msg, cid, MSG_OK);
				flash(cid);
				counter = 0;
			}
		}
	}
	app_start();
}
