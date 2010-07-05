/*
**
**	$Filename:	switch.c $
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

#include "switch.h"
#include "bit.h"

/***********/
/* Weichen */
/***********/

void switch_init_old(struct mrw_switch *device)
{
	out_pin(&device->left);
	out_pin(&device->right);
}

void switch_init_new(struct mrw_switch *device)
{
	out_pin(&device->left);
	out_pin(&device->right);
	init_input(&device->left_input,  0);
	init_input(&device->right_input, 0);
}


void switch_off(struct mrw_switch *device)
{
	clr_pin(&device->left);
	clr_pin(&device->right);
}

void switch_left(struct mrw_switch *device)
{
	set_pin(&device->left);
}

void switch_right(struct mrw_switch *device)
{
	set_pin(&device->right);
}

uint8_t switch_test(struct mrw_switch *device)
{
	if (test_state_change(&device->left_input) == PIN_FALLING_EDGE)
	{
		return SWITCH_STATE_LEFT;
	}
	else if (test_state_change(&device->right_input) == PIN_FALLING_EDGE)
	{
		return SWITCH_STATE_RIGHT;
	}

	return 0;
}

uint8_t switch_dir(struct mrw_switch *device)
{
	uint8_t type = 0;

	if (!device->left_input.last_state)
	{
		type |= SWITCH_STATE_LEFT;
	}

	if (!device->right_input.last_state)
	{
		type |= SWITCH_STATE_RIGHT;
	}
	return type;
}

/***************************/
/* 2-flügelige Formsignale */
/***************************/

void form2_init(struct mrw_form2 *signal)
{
	for (int i = 0;i < 2;i++)
	{
		out_pin(&signal->inductor[i]);
	}
}

uint8_t form2_turn(struct mrw_form2 *signal, uint8_t cmd)
{
	uint8_t pin = 0;

	switch(cmd)
	{
	case SIGNAL_HP0:
	case SIGNAL_VR0:
	case SIGNAL_SH0:
	case SIGNAL_OFF:
		set_pin(&signal->inductor[0]);
		clr_pin(&signal->inductor[1]);
		break;

	case SIGNAL_HP1:
	case SIGNAL_VR1:
	case SIGNAL_SH1:
		clr_pin(&signal->inductor[0]);
		set_pin(&signal->inductor[1]);
		pin = 1;
		break;
	}
	return pin;
}

void form2_off(struct mrw_form2 *signal)
{
	for (int i = 0;i < 2;i++)
	{
		clr_pin(&signal->inductor[i]);
	}
}

/***************************/
/* 3-flügelige Formsignale */
/***************************/

void form3_init(struct mrw_form3 *signal)
{
	for (int i = 0;i < 3;i++)
	{
		out_pin(&signal->inductor[i]);
	}
}

uint8_t form3_turn(struct mrw_form3 *signal, uint8_t cmd)
{
	uint8_t pin = 0;

	switch(cmd)
	{
	case SIGNAL_HP0:
	case SIGNAL_VR0:
	case SIGNAL_OFF:
		pin = 0;
		set_pin(&signal->inductor[0]);
		clr_pin(&signal->inductor[1]);
		clr_pin(&signal->inductor[2]);
		break;

	case SIGNAL_HP1:
	case SIGNAL_VR1:
		pin = 1;
		clr_pin(&signal->inductor[0]);
		set_pin(&signal->inductor[1]);
		clr_pin(&signal->inductor[2]);
		break;

	case SIGNAL_HP2:
	case SIGNAL_VR2:
		pin = 2;
		clr_pin(&signal->inductor[0]);
		clr_pin(&signal->inductor[1]);
		set_pin(&signal->inductor[2]);
		break;
	}
	return pin;
}

void form3_off(struct mrw_form3 *signal)
{
	for (int i = 0;i < 3;i++)
	{
		clr_pin(&signal->inductor[i]);
	}
}
