/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Neue CAN Message Verarbeitung ueber generiertes Zustandsdiagramm.
**
*/

#include "can_processing.h"
#include "CAN_Node.h"

int8_t isConfiguring(void)
{
	return get_state_CAN_Node()  == CAN_NODE__CONFIGURING;
}

int8_t isResetting(void)
{
	return get_state_CAN_Node() == CAN_NODE__RESETTING;
}
