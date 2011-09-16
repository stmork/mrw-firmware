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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "mrw.h"
#include "mcp2515.h"

#include "can_pc.h"
#include "mrw_dump.h"
#include "mrw_thread.h"

#define BUFFER_MAX 256
#define BUFFER_MASK (BUFFER_MAX-1)

static Mutex log_mutex;

class SendBuffer : public Thread, public Event
{
	int wpos;
	int rpos;
	int dst;
	const char *marker;

	receive_buffer buffer[BUFFER_MAX];

	static unsigned int Sender(void *ptr);

public:
	SendBuffer(int dst, const char *marker) : marker(marker)
	{
		wpos = 0;
		rpos = 0;
		this->dst = dst;
		Start(Sender, this, -5);
	}

	int Receive(unsigned char c)
	{
		return uart_receive(&buffer[wpos], c);
	}

	void Increment()
	{
		wpos = (wpos + 1) & BUFFER_MASK;
	}

	void Init()
	{
		uart_init(&buffer[wpos]);
	}
};

unsigned int SendBuffer::Sender(void *ptr)
{
	SendBuffer *buffer = (SendBuffer *)ptr;
	int dst = buffer->dst;
	
	do
	{
		buffer->Event::Wait();

		while(buffer->rpos != buffer->wpos)
		{
#ifdef _DEBUG
			printf("Sender %d [%d,%d]\n", dst, buffer->rpos, buffer->wpos);
#endif
			receive_buffer *send = &buffer->buffer[buffer->rpos];
			write(dst, send->input.buffer, send->index);

			{
				Lock lock(log_mutex);

				dump_mrw_msg(&send->input.can, send->checksum, buffer->marker);
			}

			buffer->rpos = (buffer->rpos + 1) & BUFFER_MASK;
		}
	}
	while(1);
}

class TransportInfo : public Thread
{
	int src;
	int dst;
	const char *marker;
	
	static unsigned int Transporter(void *);

public:
	TransportInfo(int src, int dst, const char *marker) : marker(marker)
	{
		this->src = src;
		this->dst = dst;
		Start(Transporter, this, 5);
	}
};

unsigned int TransportInfo::Transporter(void *ptr)
{
	TransportInfo *info = (TransportInfo *)ptr;

	unsigned char c[16];
	size_t read_bytes;
	SendBuffer buffer(info->dst, info->marker);

	while(1)
	{
		read_bytes = read(info->src, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;

			for (i = 0; i < read_bytes;i++)
			{
				int result = buffer.Receive(c[i]);

				switch (result)
				{
				case 1:
#ifdef _DEBUG
					printf("Retriever %d -> %d [%d,%d]\n", info->src, info->dst, buffer.rpos, buffer.wpos);
#endif
					buffer.Increment();
					buffer.Pulse();
				case -1:
					buffer.Init();
					break;
				}
			}
		}
		else if (read_bytes < 0)
		{
			perror(0);
			return -1;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

static void child(int uart, int s)
{
	TransportInfo tcp_to_uart(s, uart, "<");
	TransportInfo uart_to_tcp(uart, s, ">");

	printf("Starting TCP (%d) <-> UART (%d) communication...\n", s, uart);

	tcp_to_uart.Wait();
	uart_to_tcp.Stop();
	
	close(s);
	printf("TCP<->UART communication stopped.\n");
}
 
int main(int argc, char *argv[])
{
	pid_t reader;

	if (argc <= 1)
	{
		printf("USAGE:\n");
		printf("%s tty\n", argv[0]);
		return EXIT_SUCCESS;
	}

	int fd = uart_open(argv[1]);
	if (fd < 0)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	int s = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0)
	{
		close(fd);
		perror(0);
		return EXIT_FAILURE;
	}

	struct sockaddr_in ep_addr, client_addr;
	
	memset(&ep_addr, 0, sizeof(ep_addr));
	ep_addr.sin_family = AF_INET;
	ep_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ep_addr.sin_port = htons(4268);
	unsigned int clientlen = sizeof(client_addr);

	bind(s, (struct sockaddr *)&ep_addr, sizeof(ep_addr));

	printf("Waiting for connection...\n");
	listen(s, 5);

	do
	{
		int client = accept(s, (struct sockaddr *)&client_addr, &clientlen);
		if (client >= 0)
		{
			printf("OK, connection from %08x.\n", ntohl(client_addr.sin_addr.s_addr));

			reader = fork();
			if (reader == -1)
			{
				close(client);
				perror(0);
			}
			else if (reader == 0)
			{
				child(fd, client);
				return EXIT_SUCCESS;
			}
		}
	}
	while (1);
}	
