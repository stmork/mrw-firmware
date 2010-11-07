/*
**
**  $Revision$
**  $Date$
**  $Author$
**  $Id$
**
**	Client request handler
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

ClientHandler::ClientHandler(UartReader &reader, int s) : reader(reader)
{
	fd = s;
	Start(Writer, this);
}

unsigned int ClientHandler::Writer(void *ptr)
{
	ClientHandler *handler = (ClientHandler *)ptr;
	ReceiveBuffer *item;
	
	do
	{
		handler->Event::Wait();
		
		do
		{
			{
				Lock lock(handler->mutex);
			
				item = handler->queue.front();
				if (item != null)
				{
					handler->queue.pop();
				}
			}
			
			if (item != null)
			{
				item->Write(handler->fd);
			}
		}
		while(item != null);
	}
	while (1);
}

void ClientHandler::Add(ReceiveBuffer &buffer)
{
	Lock lock(mutex);
	ReceiveBuffer *item = new ReceiveBuffer(buffer);
	
	queue.push(item);
	Event::Pulse();
}

int ClientHandler::Reader()
{
	ReceiveBuffer  buffer;
	unsigned char  c[256];
	size_t         read_bytes;

	reader.Register(this);
	logger.Info("Starting TCP-Reader...\n");
	
	do
	{
		read_bytes = read(fd, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;

			for (i = 0; i < read_bytes;i++)
			{
				int result = buffer.Receive(c[i]);

				switch (result)
				{
				case 1:
					buffer.Dump("<");
					buffer.Write(reader.GetFD());
					buffer.Init();
					break;
				}
			}
		}
		else if (read_bytes < 0)
		{
			logger.Error("TCP-Reader");
			return -1;
		}
	}
	while(read_bytes > 0);

	logger.Info("Leaving TCP-Reader...\n");
	reader.Unregister(this);
	return 1;
}

void ClientHandler::Close()
{
	if (fd >= 0)
	{
		close(fd);
		fd = 0;
	}
}

static Thread thread("Client handler");
	
void ClientHandler::Handle(ClientHandler *handler)
{
	thread.Start(HandleTcp, handler);
}

unsigned int ClientHandler::HandleTcp(void *ptr)
{
	ClientHandler *handler = (ClientHandler *)ptr;
	
	handler->Reader();
	delete handler;
}
