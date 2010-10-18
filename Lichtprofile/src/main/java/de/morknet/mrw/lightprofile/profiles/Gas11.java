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

public class Gas11 extends LightProfile
{
	private final static int array[] =
	{
		  0,   0,   6,   8,   0,   0,  14,  16,   0,   0,  22,  24,   0,   0,  30,  32, 
		  0,   0,  38,  40,   0,   0,  46,  48,   0,   0,  54,  56,   0,   0,  62,  64,
		  0,   0,  67,  68,   0,   0,  71,  72,   0,   0,  75,  76,   0,   0,  79,  80,
		  0,   0,  83,  84,   0,   0,  87,  88,   0,   0,  91,  92,   0,   0,  95,  96,
		  0,   0,  99, 100,   0,   0, 103, 104,   0,   0, 107, 108,   0,   0, 111, 112,
		  0,   0, 115, 116,   0,   0, 119, 120,   0,   0, 123, 124,   0,   0, 127, 128,
		  0,   0, 131, 132,   0,   0, 135, 136,   0,   0, 139, 140,   0,   0, 143, 144,
		 32,  32, 147, 148,  32,  32, 151, 152,  32,  32, 155, 156,  32,  32, 159, 160,
		 64,  64, 163, 164,  64,  64, 167, 168,  64,  64, 171, 172,  64,  64, 175, 176,
		 96,  96, 179, 180,  96,  96, 183, 184,  96,  96, 187, 188,  96,  96, 191, 192,
		128, 128, 195, 196, 128, 128, 199, 200, 128, 128, 203, 204, 128, 128, 207, 208,
		160, 160 ,211, 212, 160, 160, 215, 216, 160, 160, 219, 220, 160, 160, 223, 224,
		192, 192, 226, 226, 192, 192, 228, 228, 192, 192, 230, 230, 192, 192, 232, 232,
		224, 224, 234, 234, 224, 224, 236, 236, 224, 224, 238, 238, 224, 224, 240, 240,
		240, 240, 242, 242, 240, 240, 244, 244, 240, 240, 246, 246, 240, 240, 248, 248,
		249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255
	};

	@Override
	public int [] getArray()
	{
		return array;
	}
}
