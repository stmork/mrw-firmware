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

public class Gas08 extends LightProfile
{
	private final static int array[] =
	{
		  2,   4,   0,   0,  10,  12,   0,   0,  18,  20,   0,   0,  26,  28,   0,   0,
		 34,  36,   0,   0,  42,  44,   0,   0,  50,  52,   0,   0,  58,  60,   0,   0,
		 65,  66,   0,   0,  69,  70,   0,   0,  73,  74,   0,   0,  77,  78,   0,   0,
		 81,  82,   0,   0,  85,  86,   0,   0,  89,  90,   0,   0,  93,  94,   0,   0,
		 97,  98,   0,   0, 101, 102,   0,   0, 105, 106,   0,   0, 109, 110,   0,   0,
		113, 114,   0,   0, 117, 118,   0,   0, 121, 122,   0,   0, 125, 126,   0,   0,
		129, 130,   0,   0, 133, 134,   8,   8, 137, 138,  16,  16, 141, 142,  24,  24,
		145, 146,  32,  32, 149, 150,  32,  40, 153, 154,  48,  48, 157, 158,  56,  56,
		161, 162,  64,  64, 165, 166,  72,  72, 169, 170,  80,  88, 173, 174,  88,  88,
		177, 178,  96,  96, 181, 182, 104, 104, 185, 186, 112, 112, 189, 190, 120, 120,
		193, 194, 128, 128, 197, 198, 136, 136, 201, 202, 144, 144, 205, 206, 152, 152,
		209, 210 ,160, 160, 213 ,214, 168, 168, 217, 218, 176, 176, 221, 222, 184, 184,
		225, 225, 192, 192, 227, 227, 200, 200, 229, 229, 208, 208, 231, 231, 216, 216,
		233, 233, 224, 224, 235, 235, 226, 226, 237, 237, 229, 230, 239, 239, 231, 232,
		241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 248,
		249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255
	};

	static
	{
		LightProfile.profiles.add(new Gas08());
	}

	@Override
	protected int [] getArray()
	{
		return array;
	}
}
