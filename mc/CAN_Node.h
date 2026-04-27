/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Modellrailway state chart generation CAN-Node.
**
*/

#ifndef CAN_NODE_H
#define CAN_NODE_H

#include "mcp2515.h"

#define CAN_NODE__INITIAL 0

/*******************************************************************
Dieser Zustand entspricht dem normalen Betriebsmodus. Hier werden
Steuerkommandos verarbeitet.
*******************************************************************/

#define CAN_NODE__OPERATING 1

/*******************************************************************
Im Konfigurationsmodus werden Konfigurationskommandos
verarbeitet. Dieser Zustand kann nur in einen Reset überführt
werden. Durch den Reset wird dann die hardware neu
initialisiert.
*******************************************************************/

#define CAN_NODE__CONFIGURING 2

/*******************************************************************
Dieser Zustand ist ein Übergangszustand. Es wurde ein Watchdog Reset
ausgelöst. Dieser wird erst nach einer Sekunde durchgeführt. Während
dessen ist der Mikrocontroller in diesem Zustand und kann keine Aufgaben
durchführen.
*******************************************************************/

#define CAN_NODE__RESETTING 3

/*******************************************************************
Dieser Betriebsmodus wird nach dem Booten erreicht, wenn
keine gültige Konfiguration im EEPROM abgelegt ist. Von hier
aus kann nur konfiguriert oder gebootet werden.
*******************************************************************/

#define CAN_NODE__UNCONFIGURED 4

extern int8_t get_state_CAN_Node(void);
extern void   init_CAN_Node(void);
extern void   CAN_Node(CAN_message *msg);
#endif
