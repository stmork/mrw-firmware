/*
**
**  $Filename:  mrw_daemon.c $
**  $Revision$
**  $Date$
**  $Author$
**  $Id$
**
**	UART reader
**
**  Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**  This program and the accompanying materials are made available under the
**  terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**  which accompanies this distribution.
**
**  The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#include "Log.h"
#include "ClientHandler.h"
#include "ReceiveBuffer.h"
#include "UartReader.h" 

#include <errno.h>

UartReader::UartReader(SerialLine &uart) : uart(uart), Thread("uart reader")
{
}

void UartReader::Start()
{
	Thread::Start(Reader, this, 5);
}

void UartReader::Register(ClientHandler *handler)
{
	::Lock lock(*this);

	clients.insert(handler);
}

void UartReader::Unregister(ClientHandler *handler)
{
	::Lock lock(*this);

	clients.erase(handler);
}

void UartReader::Add(ReceiveBuffer &buffer)
{
	std::set<ClientHandler *>::iterator iter;
	ClientHandler *handler;

	for(iter = clients.begin(); iter != clients.end(); iter++)
	{
		handler = *iter;
		handler->Add(buffer);
	}
}

unsigned int UartReader::Reader(void *ptr)
{
	UartReader    *reader = (UartReader *)ptr;
	ReceiveBuffer  buffer;
	uint8_t        c[256];
	size_t         read_bytes;

	logger.Info("Starting UART-Reader...\n");

	do
	{
		read_bytes = reader->uart.Read(c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;

			for (i = 0; i < read_bytes;i++)
			{
				int result = buffer.Receive(c[i]);

				switch (result)
				{
				case 1:
					buffer.Dump(">");
					reader->Add(buffer);
					buffer.Init();
					break;
				}
			}
		}
		else if (read_bytes < 0)
		{
			logger.Error("UART-Reader");
			return -1;
		}
	}
	while(read_bytes > 0);
	return 1;
}
