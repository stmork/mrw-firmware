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

public class Neon05 extends LightProfile
{
	private final static int array[] =
	{
		0,16,24,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
		128,128,128,128,128,96,96,
		128,160,255,255,255,255,
		32,32,32,32,32,32,32,
		128,160,255,255,255,255,255,
		32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
		128,160,255,255,255,255,
		32,32,32,32,32,32,32,
		128,160,255,255,255,255,255,
		144,160,176,208,240,255,255,255,
		32,32,32,32,32,32,32,32,32,32,
		255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255, 
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
