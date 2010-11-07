/*
**
**  $Filename:  mrw_daemon.c $
**  $Revision$
**  $Date$
**  $Author$
**  $Id$
**
**	RS232 <-> TCP Proxydaemon
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

#include <stdio.h>
#include <stdlib.h>

#include "ClientHandler.h"
#include "Log.h"
#include "SerialLine.h"
#include "ServerSocket.h"
#include "UartReader.h"

int main(int argc, char *argv[])
{
	logger.Info("Start\n");
	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty\n", argv[0]);
		return EXIT_SUCCESS;
	}
	SerialLine uart(argv[1]);
	UartReader reader(uart);

	ServerSocket server;

	reader.Start();
	uart.Sync();
	do
	{
		ClientHandler *handler = new ClientHandler(reader, server.Accept());
		
		ClientHandler::Handle(handler);
	}
	while(1);

	return EXIT_SUCCESS;
}
