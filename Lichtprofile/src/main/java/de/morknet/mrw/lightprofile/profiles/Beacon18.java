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

public class Beacon18 extends LightProfile
{
	private final static int array[] =
	{
		                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	 20,
	   	                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	 20,
	   	                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	 20,
	   	                                                        22,  24,  26,  24,  22,
	   	 20,  18,  16,  14,  16,  18,  20,  18,  16,  16,  16,  18,  20,  30, 128, 255,
	   	255, 128,  30,  20,  18,  16,  16,  16,  18,  20,  18,  16,  14,  16,  18,  20,
	   	 22,  24,  26,  24,  22,
	   	 20
	};

	@Override
	public int [] getArray()
	{
		return array;
	}
	
	@Override
	final protected boolean isRepeatable()
	{
		return true;
	}
}
