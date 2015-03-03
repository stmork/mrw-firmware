/*
**
**	$Revision$
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
		  0,   0, 131, 132,   8,   8, 135, 136,  16,  16, 139, 140,  24,  24, 143, 144,
		 32,  32, 147, 148,  40,  40, 151, 152,  48,  48, 155, 156,  56,  56, 159, 160,
		 64,  64, 163, 164,  72,  72, 167, 168,  80,  80, 171, 172,  88,  88, 175, 176,
		 96,  96, 179, 180, 104, 104, 183, 184, 112, 112, 187, 188, 120, 120, 191, 192,
		128, 128, 195, 196, 136, 136, 199, 200, 144, 144, 203, 204, 152, 152, 207, 208,
		160, 160 ,211, 212, 168, 168, 215, 216, 176, 176, 219, 220, 184, 184, 223, 224,
		192, 192, 226, 226, 200, 200, 228, 228, 208, 208, 230, 230, 216, 216, 232, 232,
		224, 224, 233, 234, 226, 226, 235, 236, 229, 230, 237, 238, 234, 235, 239, 239,
		241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 248,
		249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255
	};

	@Override
	protected int [] getArray()
	{
		return array;
	}
}
