/*
**
**	$Filename:	signal.c $
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

#include "signal.h"
#include "serial.h"

/*********************************************************/
/* Alle Lampen aus, egal wieviel Begriffe das Signal hat */
/*********************************************************/

static void signal_off(struct mrw_signal *signal)
{
	for (uint8_t i = 0;i < signal->count; i++)
	{
		CLR_SER_BIT(signal->byte[i], signal->bit[i]);
	}
}

/********************************************************/
/* Alle Lampen an, egal wieviel Begriffe das Signal hat */
/********************************************************/

static void signal_test(struct mrw_signal *signal)
{
	for (uint8_t i = 0;i < signal->count; i++)
	{
		SET_SER_BIT(signal->byte[i], signal->bit[i]);
	}
}

/****************************************************************/
/* 2-begriffiges Signal (Vorsignal/Blocksignal/Gleissperrsignal */
/****************************************************************/

static void signal_vorsignal2_sig0(struct mrw_signal *signal)
{
	SET_SER_BIT(signal->byte[0], signal->bit[0]); // (gelb o. rot)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // (gruen o. weiss)
}

static void signal_vorsignal2_sig1(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // (gelb o. rot)
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // (gruen o. weiss)
}

/***************************/
/* 3-begriffiges Vorsignal */
/***************************/

static void vorsignal3_vr0(struct mrw_signal *signal)
{
	SET_SER_BIT(signal->byte[0], signal->bit[0]); // li ob (gelb)
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // li un (gelb)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // re ob (gruen)
	CLR_SER_BIT(signal->byte[3], signal->bit[3]); // re un (gruen)
}

static void vorsignal3_vr1(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li ob (gelb)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // li un (gelb)
	SET_SER_BIT(signal->byte[2], signal->bit[2]); // re ob (gruen)
	SET_SER_BIT(signal->byte[3], signal->bit[3]); // re un (gruen)
}

static void vorsignal3_vr2(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li ob
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // li un
	SET_SER_BIT(signal->byte[2], signal->bit[2]); // re ob (gruen)
	CLR_SER_BIT(signal->byte[3], signal->bit[3]); // re un (gruen)
}

/*************************************/
/* Einfahrsignal (immer 3-begriffig) */
/*************************************/

static void signal3_hp0(struct mrw_signal *signal)
{
	SET_SER_BIT(signal->byte[0], signal->bit[0]); // li un (rot)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // re ob (gruen)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // re un (gelb)
}

static void signal3_hp1(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li un (rot)
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // re ob (gruen)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // re un (gelb)
}

static void signal3_hp2(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li un (rot)
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // re ob (gruen)
	SET_SER_BIT(signal->byte[2], signal->bit[2]); // re un (gelb)
}

/*************************************/
/* Ausfahrsignal (immer 4-begriffig) */
/*************************************/

static void signal4_hp00(struct mrw_signal *signal)
{
	SET_SER_BIT(signal->byte[0], signal->bit[0]); // li (rot)
	SET_SER_BIT(signal->byte[1], signal->bit[1]); // re (rot)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // mi (weiss)
	CLR_SER_BIT(signal->byte[3], signal->bit[3]); // ob (gruen)
	CLR_SER_BIT(signal->byte[4], signal->bit[4]); // un (gelb)
}

static void signal4_hp0sh1(struct mrw_signal *signal)
{
	SET_SER_BIT(signal->byte[0], signal->bit[0]); // li (rot)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // re (rot)
	SET_SER_BIT(signal->byte[2], signal->bit[2]); // mi (weiss)
	CLR_SER_BIT(signal->byte[3], signal->bit[3]); // ob (gruen)
	CLR_SER_BIT(signal->byte[4], signal->bit[4]); // un (gelb)
}

static void signal4_hp1(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li (rot)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // re (rot)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // mi (weiss)
	SET_SER_BIT(signal->byte[3], signal->bit[3]); // ob (gruen)
	CLR_SER_BIT(signal->byte[4], signal->bit[4]); // un (gelb)
}

static void signal4_hp2(struct mrw_signal *signal)
{
	CLR_SER_BIT(signal->byte[0], signal->bit[0]); // li (rot)
	CLR_SER_BIT(signal->byte[1], signal->bit[1]); // re (rot)
	CLR_SER_BIT(signal->byte[2], signal->bit[2]); // mi (weiss)
	SET_SER_BIT(signal->byte[3], signal->bit[3]); // ob (gruen)
	SET_SER_BIT(signal->byte[4], signal->bit[4]); // un (gelb)
}

/***************************/
/* Dispatcher fuer Signale */
/***************************/

static void set_signal2(struct mrw_signal *signal)
{
	switch (signal->img)
	{
	case SIGNAL_OFF:
		signal_off(signal);
		break;
	case SIGNAL_TST:
		signal_test(signal);
		break;

	case SIGNAL_VR0:
	case SIGNAL_HP0:
	case SIGNAL_SH0:
		signal_vorsignal2_sig0(signal);
		break;

	case SIGNAL_VR1:
	case SIGNAL_HP1:
	case SIGNAL_SH1:
		signal_vorsignal2_sig1(signal);
		break;
	}
}

static void set_signal3(struct mrw_signal *signal)
{
	switch (signal->img)
	{
	case SIGNAL_OFF:
		signal_off(signal);
		break;
	case SIGNAL_TST:
		signal_test(signal);
		break;
	case SIGNAL_VR0:
		vorsignal3_vr0(signal);
		break;
	case SIGNAL_VR1:
		vorsignal3_vr1(signal);
		break;
	case SIGNAL_VR2:
		vorsignal3_vr2(signal);
		break;
	case SIGNAL_HP0:
		signal3_hp0(signal);
		break;
	case SIGNAL_HP1:
		signal3_hp1(signal);
		break;
	case SIGNAL_HP2:
		signal3_hp2(signal);
		break;
	}
}

static void set_signal4(struct mrw_signal *signal)
{
	switch (signal->img)
	{
	case SIGNAL_OFF:
		signal_off(signal);
		break;
	case SIGNAL_TST:
		signal_test(signal);
		break;
	case SIGNAL_SH1:
		signal4_hp0sh1(signal);
		break;
	case SIGNAL_HP0:
		signal4_hp00(signal);
		break;
	case SIGNAL_HP1:
		signal4_hp1(signal);
		break;
	case SIGNAL_HP2:
		signal4_hp2(signal);
		break;
	}
}

static void set_simple_light(struct mrw_simple_light *light)
{
	if (light->on)
	{
		SET_SER_BIT(light->byte, light->bit);
	}
	else
	{
		CLR_SER_BIT(light->byte, light->bit);
	}
}

void compute_signal(mrw_device *dvc)
{
	switch(dvc->unit_type)
	{
	case TYPE_SIGNAL_PL2:
	case TYPE_SIGNAL_ML2:
	case TYPE_SIGNAL_SL2:
		set_signal2(&dvc->unit.u_signal);
		break;

	case TYPE_SIGNAL_PL3:
	case TYPE_SIGNAL_ML3:
		set_signal3(&dvc->unit.u_signal);
		break;
		
	case TYPE_SIGNAL_ML4:
		set_signal4(&dvc->unit.u_signal);
		break;

	case TYPE_SIMPLE_LIGHT:
		set_simple_light(&dvc->unit.u_simple_light);
		break;
	}
}
