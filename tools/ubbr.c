/*
**
**	$Filename:	ubbr.c $
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Compute UBBR baud error rate.
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
#include <math.h>

//#define MHZ 10
//#define MHZ 12
//#define MHZ 16
#define MHZ 14.7456
//#define MHZ 9.216

#define F_CPU ((long)(MHZ * 1000000))

#define UBRR_VAL(F_CPU)      (F_CPU       / (BAUD << 4) - 1)
#define REAL(F_CPU)          (F_CPU * 100 / (BAUD << 4))
#define BAUD_ERROR           (((UBRR_VAL + 2) * 100 - REAL) % 100)

static double compute_ubbr(int baud, double hz)
{
	return hz / (baud << 4) - 1;
}

int main(int argc, char * argv[])
{
	int baud = 115200;
	int i;

	for (i = 1; i < 8; i++)
	{
		double hz    = MHZ * 250000.0 * i;
		double ubbr  = compute_ubbr(baud, hz);
		double error = fabs(((floor(ubbr) + 1) / (ubbr + 1.0) - 1) * 100.0);

		printf("%2.6f MHz %6d baud: ubbr: %d error: %1.4f%%\n", hz / 1000000.0, baud, (int)ubbr, error);
	}
	return EXIT_SUCCESS;
}
