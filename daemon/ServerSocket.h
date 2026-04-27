/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	TCP connection handler
**
*/

#pragma once

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <netinet/ip.h>

class ServerSocket
{
	int s;
	struct sockaddr_in client_addr;
	unsigned int clientlen;

public:
	ServerSocket();
	virtual ~ServerSocket()
	{
		Close();
	}

	int Accept();

private:
	void Close();
};

#endif
