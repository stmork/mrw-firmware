/*
**
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Receive buffer handling
**
**	(C) Copyright 2010  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef RECEIVE_BUFFER_H
#define RECEIVE_BUFFER_H

#include "mcp2515.h"
#include "Log.h"
#include "UartReader.h"

class ReceiveBuffer
{
	union
	{
		CAN_message can;
		uint8_t     buffer[15];
	}       input;
	uint8_t index;
	uint8_t checksum;

public:
	inline ReceiveBuffer()
	{
		Init();
	}

	inline ReceiveBuffer(ReceiveBuffer &buffer)
	{
		input = buffer.input;
		index = buffer.index;
		checksum = buffer.checksum;
	}

	inline int Receive(uint8_t b)
	{
		int complete = 0;

		input.buffer[index++] = b;
		checksum += b;

		if ((input.can.length == 0) || (input.can.length > 8))
		{
#ifdef _DEBUG
			Lock lock(logger);
			time_t now = time(&now);

			printf("%ld # Invalid CAN length: %02x\n", now, input.can.length);
#endif
			Init();
			complete = -1;
		}
		else if ((input.can.length + offsetof(CAN_message, data) + 1) == index)
		{	
			if (checksum == 0)
			{
				complete = 1;
			}
			else
			{
				Lock lock(logger);

				time_t now = time(&now);
				int i;

				printf("%ld # Checksum error [%02x]: ", now, checksum);
				for (i = 0;i < index;i++)
				{
					printf(" %02x", input.buffer[i]);
				}
				printf("\n");
				fflush(stdout);
				Init();
				complete = -1;
			}
		}
		return complete;
	}

	inline void Dump(const char *comment)
	{
		logger.Dump(&input.can, checksum, comment);
	}

	inline int Write(int fd)
	{
		return write(fd, input.buffer, index);
	}

	inline void Init()
	{
		bzero(&input, sizeof(input));
		index    = 0;
		checksum = 0;
	}

};

#endif
