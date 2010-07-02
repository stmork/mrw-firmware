/*
**
**	$Filename:	can_pc.h $
**	$Revision: 966 $
**	$Date: 2010-06-06 13:54:27 +0200 (So, 06. Jun 2010) $
**	$Author: smork $
**	$Id: can_pc.h 966 2010-06-06 11:54:27Z smork $
**
**	RS232/CAN frame handler
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

#ifndef UART_PC_H
#define UART_PC_H

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

#include "mcp2515.h"

#define MAX_CAN_LENGTH (offsetof(CAN_message, data[8])+1)

typedef struct
{
	union
	{
		CAN_message can;
		uint8_t     buffer[15];
	}       input;
	uint8_t index;
	uint8_t checksum;
} receive_buffer;

extern int  uart_open(char *name);
extern void uart_dump_can_msg(CAN_message *msg, unsigned char sum, char *comment);
extern void uart_sync(int fd);
extern int  uart_send_can_msg(int fd, CAN_message *msg);
extern int  uart_send_can_data(int fd, unsigned short id, unsigned char *buffer, int length);
extern int  uart_send_ext_can_data(int fd, unsigned long id, unsigned char *buffer, int length);

extern void  uart_init(receive_buffer *buffer);
extern int   uart_receive(receive_buffer *buffer,uint8_t byte);

extern void  can_fill_message(CAN_message *msg, uint8_t cmd, uint16_t id, uint16_t no);
extern void  can_add_data(CAN_message *msg, uint8_t data);

#endif
