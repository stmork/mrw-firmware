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

struct transport_info
{
	int src;
	int dst;
};

#define BUFFER_MAX 256
#define BUFFER_MASK (BUFFER_MAX-1)

typedef struct
{
	int wpos;
	int rpos;
	int dst;
	pthread_mutex_t mutex;
	pthread_cond_t  event;

	receive_buffer buffer[BUFFER_MAX];
} send_buffer;

static pthread_mutex_t log_mutex;

static void *sender(void *ptr)
{
	send_buffer *buffer = (send_buffer *)ptr;
	int dst = buffer->dst;
	
	do
	{
		pthread_mutex_lock(&buffer->mutex);
		pthread_cond_wait(&buffer->event,&buffer->mutex);
		pthread_mutex_unlock(&buffer->mutex);

		while(buffer->rpos != buffer->wpos)
		{
#ifdef _DEBUG
			printf("Sender %d [%d,%d]\n", dst, buffer->rpos, buffer->wpos);
#endif
			receive_buffer *send = &buffer->buffer[buffer->rpos];
			write(dst, send->input.buffer, send->index);

			pthread_mutex_lock(&log_mutex);
			dump_mrw_msg(&send->input.can, send->checksum, "<");
			pthread_mutex_unlock(&log_mutex);

			buffer->rpos = (buffer->rpos + 1) & BUFFER_MASK;
		}
	}
	while(1);
}

static void *transport(void *ptr)
{
	struct transport_info *info = (struct transport_info *)ptr;

	unsigned char c[16];
	size_t read_bytes;
	send_buffer buffer;
	pthread_t sender_thread;

	buffer.rpos  = 0;
	buffer.wpos  = 0;
	buffer.dst   = info->dst;
	pthread_mutex_init(&buffer.mutex, NULL);
	pthread_cond_init(&buffer.event, NULL);

	pthread_create(&sender_thread, NULL, sender, &buffer);

	while(1)
	{
		read_bytes = read(info->src, c, sizeof(c));
		if (read_bytes > 0)
		{
			int i;

			for (i = 0; i < read_bytes;i++)
			{
				int result = uart_receive(&buffer.buffer[buffer.wpos], c[i]);
				switch (result)
				{
				case 1:
#ifdef _DEBUG
					printf("Retriever %d -> %d [%d,%d]\n", info->src, info->dst, buffer.rpos, buffer.wpos);
#endif
					pthread_mutex_lock(&buffer.mutex);
					pthread_cond_broadcast(&buffer.event);
					pthread_mutex_unlock(&buffer.mutex);

					buffer.wpos = (buffer.wpos + 1) & BUFFER_MASK;
				case -1:
					uart_init(&buffer.buffer[buffer.wpos]);
					break;
				}
			}
		}
		else if (read_bytes < 0)
		{
			perror(0);
			return;
		}
	}
}

static void child(int uart, int s)
{
	struct transport_info tcp_to_uart_info;
	struct transport_info uart_to_tcp_info;
	
	pthread_t tcp_to_uart;
	pthread_t uart_to_tcp;

	printf("Starting TCP<->UART communication...\n");
	uart_to_tcp_info.src = uart;
	uart_to_tcp_info.dst = s;
	pthread_create(&uart_to_tcp, NULL, transport, &uart_to_tcp_info);

	tcp_to_uart_info.src = s;
	tcp_to_uart_info.dst = uart;
	pthread_create(&tcp_to_uart, NULL, transport, &tcp_to_uart_info);

//	pthread_join(uart_to_tcp, NULL);
	pthread_join(tcp_to_uart, NULL);
	
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

	pthread_mutex_init(&log_mutex, NULL);

	do
	{
		int client = accept(s, (struct sockaddr *)&client_addr, &clientlen);
		if (client >= 0)
		{
			printf("OK, connection from %08lx.\n", ntohl(client_addr.sin_addr.s_addr));

			reader = fork();
			if (reader == -1)
			{
				close(client);
				perror(0);
			}
			else if (reader == 0)
			{
				child(fd, client);
			}
		}
	}
	while (1);
}	
