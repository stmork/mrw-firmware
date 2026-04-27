/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	Client request handler
**
*/

#pragma once

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <queue>

#include "mrw_thread.h"

#include "ReceiveBuffer.h"

class UartReader;

class ClientHandler : public Thread, public Event
{
	Mutex        mutex;
	int          fd;
	UartReader & reader;

	std::queue<ReceiveBuffer *> queue;

public:
	ClientHandler(UartReader & reader, int s);

	inline virtual ~ClientHandler()
	{
		Close();
	}

	void Add(ReceiveBuffer & buffer);
	static void Handle(ClientHandler * handler);

private:
	static unsigned int HandleTcp(void *);
	static unsigned int Writer(void * ptr);
	int Reader();
	void Close();
};

#endif
