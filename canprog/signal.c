/*
**
**	$Filename:	signal.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Reset CAN nodes.
**
**	Copyright (C) 2010 committers of this modelrailway project. All rights reserved.
**
**	This program and the accompanying materials are made available under the
**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0
**	which accompanies this distribution.
**
**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
**
**
*/

#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h>

#include "mrw.h"  
#include "mcp2515.h"

#include "can_pc.h"
#include "testdef.h"

static void turn_signal(int fd, int ml_signal, int pl_signal1, int pl_signal2, int sh_signal)
{
	CAN_message msg;
	
	msg.sid    = TEST_SID;
	msg.length = 2;
	msg.status = 1 << FRAME_EXT;
	msg.data[0] = SETSGN;

	msg.eid    = TEST_MAIN_SIGNAL;
	msg.data[1] = ml_signal;
	uart_send_can_msg(fd, &msg);

	msg.eid    = TEST_PRE_SIGNAL1;
	msg.data[1] = pl_signal1;
	uart_send_can_msg(fd, &msg);

	msg.eid    = TEST_PRE_SIGNAL2;
	msg.data[1] = pl_signal2;
	uart_send_can_msg(fd, &msg);

	msg.eid    = TEST_RAIL_SIGNAL;
	msg.data[1] = sh_signal;
	uart_send_can_msg(fd, &msg);
}


int main(int argc,char *argv[])
{
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

	uart_sync(fd);

	do
	{
		turn_signal(fd, SIGNAL_TST, SIGNAL_TST, SIGNAL_TST, SIGNAL_TST);
		sleep(2);
		turn_signal(fd, SIGNAL_HP0, SIGNAL_OFF, SIGNAL_VR0, SIGNAL_SH0);
		sleep(1);
		turn_signal(fd, SIGNAL_HP1, SIGNAL_VR0, SIGNAL_VR1, SIGNAL_SH1);
		sleep(1);
		turn_signal(fd, SIGNAL_HP1, SIGNAL_VR1, SIGNAL_VR1, SIGNAL_SH0);
		sleep(1);
		turn_signal(fd, SIGNAL_HP1, SIGNAL_VR2, SIGNAL_VR1, SIGNAL_SH1);
		sleep(1);
		turn_signal(fd, SIGNAL_HP2, SIGNAL_VR0, SIGNAL_VR2, SIGNAL_SH0);
		sleep(1);
		turn_signal(fd, SIGNAL_HP2, SIGNAL_VR1, SIGNAL_VR2, SIGNAL_SH1);
		sleep(1);
		turn_signal(fd, SIGNAL_HP2, SIGNAL_VR2, SIGNAL_VR2, SIGNAL_SH0);
		sleep(1);
		turn_signal(fd, SIGNAL_OFF, SIGNAL_OFF, SIGNAL_OFF, SIGNAL_OFF);
		sleep(2);
	}
	while(1);

	close(fd);
	return EXIT_SUCCESS;
}
