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
		  0, 130, 131,   0,   0, 134, 135,   0,   0, 138, 139,   0,   0, 142, 143,   0,
		 32, 146, 147,  32,  32, 150, 151,  32,  32, 154, 155,  32,  32, 158, 159,  32,
		 64, 162, 163,  64,  64, 166, 167,  64,  64, 170, 171,  64,  64, 174, 175,  64,
		 96, 178, 179,  96,  96, 182, 183,  96,  96, 186, 187,  96,  96, 190, 191,  96,
		128, 194, 195, 128, 128, 198, 199, 128, 128, 202, 203, 128, 128, 206, 207, 128,
		160, 210 ,211, 160, 160, 214, 215, 160, 160, 218, 219, 160, 160, 222, 223, 160,
		192, 225, 226, 192, 192, 227, 228, 192, 192, 229, 230, 192, 192, 231, 232, 192,
		224, 233, 234, 224, 224, 235, 236, 224, 224, 237, 238, 224, 224, 239, 240, 224,
		240, 241, 242, 240, 240, 243, 244, 240, 240, 245, 246, 240, 240, 247, 248, 240,
		249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255
	};

	@Override
	public int [] getArray()
	{
		return array;
	}
}
