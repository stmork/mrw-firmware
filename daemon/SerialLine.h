/*
**
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Serial line handler
**
**	(C) Copyright 2010  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef SERIAL_LINE_H
#define SERIAL_LINE_H

#include <unistd.h>

class ClientHandler;
class SerialLine
{
	int fd;

public:
	SerialLine(const char *device);

	virtual ~SerialLine()
	{
		Close();
	}

	inline size_t Read(void *buffer, size_t count)
	{
		return read(fd, buffer, count);
	}

	inline int GetFD()
	{
		return fd;
	}

	void Sync();
private:
	void Close();
};

#endif
