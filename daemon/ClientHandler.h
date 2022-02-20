/*
**
**	Client request handler
**
**	(C) Copyright 2010-2022  Steffen A. Mork
**	    All Rights Reserved
**
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
