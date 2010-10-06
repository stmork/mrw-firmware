/*
**
**	$Filename:	test_light.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Light dimming test firmware
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

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "busy.h"
#include "config.h"
#include "light.h"
#include "light_profile.h"
#include "mcp2515.h"
#include "pwm.h"
#include "sleep.h"
#include "timer.h"

/*
 * Hier wird das Soft-PWM durchgeführt.
 */
ISR(TIMER1_COMPA_vect)
{
	BUSY;
	uint8_t i;

	for (i = 0;i < config.count; i++)
	{
		if (config.dvc[i].unit_type == TYPE_LIGHT)
		{
			handle_pwm(&config.dvc[i].unit.u_light);
		}
	}
}

/*
 * Hier wird die Helligkeit eines jeden Soft-PWM-Kanals einzeln eingestellt.
 */
ISR(TIMER2_OVF_vect)
{
	BUSY;
	mrw_device *dvc = config.dvc;
	uint8_t     i;

	for (i = 0; i < config.count;i++)
	{
		if (dvc->unit_type == TYPE_LIGHT)
		{
			light_dimm(&dvc->unit.u_light);
		}
		dvc++;
	}
}

static void config_init(void)
{
	uint8_t i;

	for (i = 0;i < 8;i++)
	{
		config.dvc[i].unit_no                = i;
		config.dvc[i].unit_type              = TYPE_LIGHT;
		config.dvc[i].unit.u_light.threshold = 0;
		config.dvc[i].unit.u_light.type      = 1;
		config_connection(&config.dvc[i].unit.u_light.pin, i);
		config.count++;
	}
	config.id    = 0x7fd;
	config.magic = CONFIG_MAGIC;

	for (i = 0;i < config.count;i++)
	{
		light_init(&config.dvc[i].unit.u_light);
	}
}

int main(void)
{
	/* Takt initialisieren, so dass MCP2515 seine 16 MHz auch her gibt. */
	/* Ansonsten passiert hier heute per CAN-Bus nix ;-) */
	config_init();
	mcp2515_init(config.id, 1, MCP2515_SINGLE_TX_BUFFER);
	timer1_init(F_CPU / (50 * PWM_TABLE_SIZE));
	timer2_init();

	do
	{
		/*
		 * Ein bißchen was für die Umwelt tun. Es fängt halt
		 * halt schon im Kleinen an ;-)
		 */
		IDLE;
		sleep();
		BUSY;
	}
	while(1);
}
