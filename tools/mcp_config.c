/*
**
**	$Filename:	mcp_config.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Compute MCP2515 configuration bytes.
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

#define TQ_COUNT        16
#define Fosc      16000000L
#define Fcan        125000L

#define SYNC_SEG  1
#define SJW       2

static void config(int khz)
{
	double Tbaud = 1.0 / (khz * 1000.0);
	double TQ = Tbaud / TQ_COUNT;
	double BRP = TQ * Fosc * 0.5 - 1;

	int PS2 = TQ_COUNT * 3 / 10;
	int PS1 = PS2;
	int PropSeg = TQ_COUNT - PS2 - PS1 - SYNC_SEG;


	unsigned char CNF1, CNF2, CNF3;

	CNF1 = (int)BRP | ((SJW - 1) << 6);
	CNF2 = ((PropSeg - 1) & 0x7) | (((PS1 - 1) & 0x7) << 3) | 0x40 | 0x80;
	CNF3 = (PS2 - 1) & 0x7;
	printf("%4d kHz: TQ=%3.3f ns BRP=%f PRS=%d PS1=%d PS2=%d # CNF1=$%02x CNF2=$%02x CNF3=$%02x\n",
		khz, TQ * 1000000000, BRP, PropSeg, PS1, PS2, CNF1, CNF2, CNF3);
}

#define BTLMODE 7
#define SAM     6
#define R_BRP   (((Fosc / (TQ_COUNT * Fcan)) >> 1) - 1)
#define R_PS2      (TQ_COUNT * 3 / 10)
#define R_PS1      R_PS2
#define PROP_SEG   (TQ_COUNT - SYNC_SEG - R_PS1 - R_PS2)

#define R_CNF1  (R_BRP | ((SJW - 1) << 6))
#define R_CNF2  ((PROP_SEG - 1) | ((R_PS1 - 1) << 3) | (1 << SAM) | (1 << BTLMODE))
#define R_CNF3  (R_PS2 - 1)

int main(int argc, char * argv[])
{
	int khz;

	for (khz = 125; khz <= 1000; khz *= 2)
	{
		config(khz);
	}
	printf("---\n");
	printf("%4ld kHz: BRP=%ld PRS=%d PS1=%d PS2=%d # CNF1=$%02lx CNF2=$%02x CNF3=$%02x\n",
		Fcan / 1000, R_BRP, PROP_SEG, R_PS1, R_PS2, R_CNF1, R_CNF2, R_CNF3);
	return EXIT_SUCCESS;
}
