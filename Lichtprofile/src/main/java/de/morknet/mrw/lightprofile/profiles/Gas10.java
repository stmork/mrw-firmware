/*
**
**	$Revision$
**	$Date$
**	$Author$
**	$Id$
**
**	Light dimming profiles
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

package de.morknet.mrw.lightprofile.profiles;

import de.morknet.mrw.lightprofile.LightProfile;

public class Gas10 extends LightProfile
{
	private final static int array[] =
	{
		  0,   4,   6,   0,   0,  12,  14,   0,   0,  20,  22,   0,   0,  28,  30,   0,
		  0,  36,  38,   0,   0,  44,  46,   0,   0,  52,  54,   0,   0,  60,  62,   0,
		  0,  66,  67,   0,   0,  70,  71,   0,   0,  74,  75,   0,   0,  78,  79,   0,
		  0,  82,  83,   0,   0,  86,  87,   0,   0,  90,  91,   0,   0,  94,  95,   0,
		  0,  98,  99,   0,   0, 102, 103,   0,   0, 106, 107,   0,   0, 110, 111,   0,
		  0, 114, 115,   0,   0, 118, 119,   0,   0, 122, 123,   0,   0, 126, 127,   0,
		  0, 130, 131,   8,   8, 134, 135,  16,  16, 138, 139,  24,  24, 142, 143,  32,
		 32, 146, 147,  40,  40, 150, 151,  48,  48, 154, 155,  56,  56, 158, 159,  64,
		 64, 162, 163,  72,  72, 166, 167,  80,  80, 170, 171,  88,  88, 174, 175,  96,
		 96, 178, 179, 104, 104, 182, 183, 112, 112, 186, 187, 120, 120, 190, 191, 128,
		128, 194, 195, 136, 136, 198, 199, 144, 144, 202, 203, 152, 152, 206, 207, 160,
		160, 210 ,211, 168, 168, 214, 215, 176, 176, 218, 219, 184, 184, 222, 223, 192,
		192, 225, 226, 200, 200, 227, 228, 208, 208, 229, 230, 216, 216, 231, 232, 224,
		224, 233, 234, 226, 226, 235, 236, 229, 230, 237, 238, 234, 235, 239, 239, 240,
		240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248,
		248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255
	};

	@Override
	protected int [] getArray()
	{
		return array;
	}
}
