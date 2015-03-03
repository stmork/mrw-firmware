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

public class Neon03 extends LightProfile
{
	private final static int array[] =
	{
		0,16,24,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
		128,160,255,255,255,255,
		32,32,32,32,32,
		128,160,255,255,
		32,32,32,
		128,160,255,255,
		32,32,32,32,
		128,160,255,255,
		32,32,32,
		128,160,255,
		32,32,32,32,
		128,160,255,255,255,255,255,255,255,255,
		32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
		128,136,144,152,
		160,168,176,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,242,244,
		246,248,249,250,251,252,253,253,254,254,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
	};

	@Override
	protected int [] getArray()
	{
		return array;
	}
}
