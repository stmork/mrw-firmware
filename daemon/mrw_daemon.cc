/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	RS232 <-> TCP Proxydaemon
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "ClientHandler.h"
#include "Log.h"
#include "SerialLine.h"
#include "ServerSocket.h"
#include "UartReader.h"

int main(int argc, char * argv[])
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
		ClientHandler * handler = new ClientHandler(reader, server.Accept());

		ClientHandler::Handle(handler);
	}
	while (1);

	return EXIT_SUCCESS;
}
