/*
**
**	UART reader
**
**	(C) Copyright 2010-2022 committers of this modelrailway project. All rights reserved.
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef UART_READER_H
#define UART_READER_H

#include <set>

#include "SerialLine.h"
#include "mrw_thread.h"

class ClientHandler;
class ReceiveBuffer;

class UartReader : public Thread, public Mutex
{
	SerialLine        &        uart;
	std::set<ClientHandler *>  clients;

public:
	UartReader(SerialLine & uart);

	void Register(ClientHandler * handler);
	void Unregister(ClientHandler * handler);
	void Start();

	inline int  GetFD() const
	{
		return uart.GetFD();
	}

private:
	static unsigned int Reader(void * ptr);
	void         Add(ReceiveBuffer & buffer);
};

#endif
