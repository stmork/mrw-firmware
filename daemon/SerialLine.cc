/*
**
**	Serial line handler
**
**  Copyright (C) 2010-2022 committers of this modelrailway project. All rights reserved.
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
#include "SerialLine.h"

#include <stdio.h>
#include <stddef.h>
#include <termios.h>
#include <fcntl.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#ifdef __linux__
#	include <asm/ioctls.h>
#	include <linux/serial.h>
#endif

//#define BAUD 230400
#define BAUD B115200
//#define BAUD B57600

#include "ClientHandler.h"
#include "Log.h"

SerialLine::SerialLine(const char * device)
{
	struct termios oldtio, newtio;

	fd = open(device, O_RDWR | O_NOCTTY);
	if (fd >= 0)
	{
		tcgetattr(fd, &oldtio);
		bzero(&newtio, sizeof(newtio));
		newtio.c_cflag = BAUD | CS8 | CLOCAL | CREAD;
		newtio.c_iflag = IGNPAR;
		newtio.c_oflag = 0;
		newtio.c_lflag = 0;
		newtio.c_cc[VTIME]    = 0;
		newtio.c_cc[VMIN]     = 1;
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd, TCSANOW, &newtio);

#ifdef TIOCGSERIAL
		struct serial_struct serinfo;

		if (ioctl (fd, TIOCGSERIAL, &serinfo) == 0)
		{
			serinfo.flags |= ASYNC_LOW_LATENCY;
			if (ioctl (fd, TIOCSSERIAL, &serinfo) != 0)
			{
				logger.Error("Setting serial flags");
			}
		}
		else
		{
			logger.Error("Getting serial flags");
		}
#endif
	}
}

void SerialLine::Sync()
{
	uint8_t byte = 0;
	int     i;

	logger.Info("Synchronizing serial connection...\n");
	for (i = 0; i < 16; i++)
	{
		if (write(fd, &byte, sizeof(byte)) != sizeof(byte))
		{
			logger.Error("Writing byte %02x", byte);
		}
	}
}

void SerialLine::Close()
{
	if (fd != 0)
	{
		close(fd);
		fd = 0;
	}
}
