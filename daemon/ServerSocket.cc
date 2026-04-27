/*
**
**	SPDX-License-Identifier: MIT
**	SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
**
**	TCP connection handler
**
**  Copyright (C) 2010-2022 committers of this modelrailway project. All rights reserved.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "ServerSocket.h"

ServerSocket::ServerSocket()
{
	struct sockaddr_in ep_addr;

	s = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0)
	{
		perror(0);
		throw;
	}

	memset(&ep_addr, 0, sizeof(ep_addr));
	ep_addr.sin_family = AF_INET;
	ep_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ep_addr.sin_port = htons(4268);
	clientlen = sizeof(client_addr);

	bind(s, (struct sockaddr *)&ep_addr, sizeof(ep_addr));

	listen(s, 5);
}

int ServerSocket::Accept()
{
	printf("Waiting for connection...\n");
	return accept(s, (struct sockaddr *)&client_addr, &clientlen);
}

void ServerSocket::Close()
{
}
