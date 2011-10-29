/*
**
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Logger
**
**	(C) Copyright 2010  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef LOG_H
#define LOG_H

#include <map>

#include "mcp2515.h"
#include "mrw.h"
#include "mrw_thread.h"

typedef std::map<const uint8_t, const char *> mrw_constant_map;

class Log : public Mutex
{
	unsigned int received;

	mrw_constant_map command_map;
	mrw_constant_map result_map;
	mrw_constant_map signal_map;

public:
	     Log();
	void Dump(const CAN_message *msg, uint8_t checksum, const char *comment);
	void Info(const char *message, ...);
	void Error(const char *message, ...);

private:
	const char* find_cmd_text(uint8_t code);
	const char* find_result_text(uint8_t code);
	const char* find_signal_text(uint8_t code);
};

extern Log logger;

#endif
