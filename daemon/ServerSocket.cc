/*
**
**  $Revision$
**  $Date$
**  $Author$
**  $Id$
**
**	TCP connection handler
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
