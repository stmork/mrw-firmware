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

public class Gas09 extends LightProfile
{
	private final static int array[] =
	{
		  2,   0,   0,   8,  10,   0,   0,  16,  18,   0,   0,  24,  26,   0,   0,  32,
		 34,   0,   0,  40,  42,   0,   0,  48,  50,   0,   0,  56,  58,   0,   0,  64,
		 65,   0,   0,  68,  69,   0,   0,  72,  73,   0,   0,  76,  77,   0,   0,  80,
		 81,   0,   0,  84,  85,   0,   0,  88,  89,   0,   0,  92,  93,   0,   0,  96,
		 97,   0,   0, 100, 101,   0,   0, 104, 105,   0,   0, 108, 109,   0,   0, 112,
		113,   0,   0, 116, 117,   0,   0, 120, 121,   0,   0, 124, 125,   0,   0, 128,
		129,   0,   0, 132, 133,   0,   0, 136, 137,   0,   0, 140, 141,   0,   0, 144,
		145,  32,  32, 148, 149,  32,  32, 152, 153,  32,  32, 156, 157,  32,  32, 160,
		161,  64,  64, 164, 165,  64,  64, 168, 169,  64,  64, 172, 173,  64,  64, 176,
		177,  96,  96, 180, 181,  96,  96, 184, 185,  96,  96, 188, 189,  96,  96, 192,
		193, 128, 128, 196, 197, 128, 128, 200, 201, 128, 128, 204, 205, 128, 128, 208,
		209, 160 ,160, 212, 213 ,160, 160, 216, 217, 160, 160, 220, 221, 160, 160, 224,
		225, 192, 192, 226, 227, 192, 192, 228, 229, 192, 192, 230, 231, 192, 192, 232,
		233, 224, 224, 234, 235, 224, 224, 236, 237, 224, 224, 238, 239, 224, 224, 240,
		241, 240, 240, 242, 243, 240, 240, 244, 245, 240, 240, 246, 247, 240, 240, 248,
		249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255
	};

	@Override
	public int [] getArray()
	{
		return array;
	}
}
