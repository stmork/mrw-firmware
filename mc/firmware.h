/*
**
**	$Filename:	firmware.h  $
**	$Revision: 931 $
**	$Date: 2010-04-14 10:39:15 +0200 (Mi, 14. Apr 2010) $
**	$Author: smork $
**	$Id: firmware.h 931 2010-04-14 08:39:15Z smork $
**
**	Konstanten der CAN-Knoten Firmware.
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

#ifndef FIRMWARE_H
#define FIRMWARE_H

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#define RING_SIZE      32
#define RING_MASK     (RING_SIZE - 1)

extern struct eeprom_area config;

#endif
