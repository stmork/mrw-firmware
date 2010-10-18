/*
**
**	$Filename:	LightProfile.java $
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

package de.morknet.mrw.lightprofile;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashSet;
import java.util.Set;

import javax.imageio.ImageIO;

import de.morknet.mrw.lightprofile.profiles.Beacon16;
import de.morknet.mrw.lightprofile.profiles.Beacon17;
import de.morknet.mrw.lightprofile.profiles.Beacon18;
import de.morknet.mrw.lightprofile.profiles.Beacon19;
import de.morknet.mrw.lightprofile.profiles.Blink12;
import de.morknet.mrw.lightprofile.profiles.Fast15;
import de.morknet.mrw.lightprofile.profiles.Gas08;
import de.morknet.mrw.lightprofile.profiles.Gas09;
import de.morknet.mrw.lightprofile.profiles.Gas10;
import de.morknet.mrw.lightprofile.profiles.Gas11;
import de.morknet.mrw.lightprofile.profiles.Medium14;
import de.morknet.mrw.lightprofile.profiles.Neon00;
import de.morknet.mrw.lightprofile.profiles.Neon01;
import de.morknet.mrw.lightprofile.profiles.Neon02;
import de.morknet.mrw.lightprofile.profiles.Neon03;
import de.morknet.mrw.lightprofile.profiles.Neon04;
import de.morknet.mrw.lightprofile.profiles.Neon05;
import de.morknet.mrw.lightprofile.profiles.Neon06;
import de.morknet.mrw.lightprofile.profiles.Neon07;
import de.morknet.mrw.lightprofile.profiles.Slow13;

public abstract class LightProfile
{
	protected final static Set<LightProfile> profiles = new HashSet<LightProfile>();
	private final BufferedImage image;
	private final float X_SCALE = 2.0f;
	private final float Y_SCALE = 0.5f;
	private final int   X_OFFSET = 8;
	private final int   Y_OFFSET = 8;

	static 
	{
		profiles.add(new Neon00());
		profiles.add(new Neon01());
		profiles.add(new Neon02());
		profiles.add(new Neon03());
		profiles.add(new Neon04());
		profiles.add(new Neon05());
		profiles.add(new Neon06());
		profiles.add(new Neon07());
		profiles.add(new Gas08());
		profiles.add(new Gas09());
		profiles.add(new Gas10());
		profiles.add(new Gas11());
		profiles.add(new Blink12());
		profiles.add(new Slow13());
		profiles.add(new Medium14());
		profiles.add(new Fast15());
		profiles.add(new Beacon16());
		profiles.add(new Beacon17());
		profiles.add(new Beacon18());
		profiles.add(new Beacon19());
	}
	
	protected LightProfile()
	{
		image = new BufferedImage(
				getWidth() + X_OFFSET * 2,
				getHeight() + Y_OFFSET * 2,
				BufferedImage.TYPE_INT_RGB);
	}
	
	final int getWidth()
	{
		return (int) (X_SCALE * getArray().length);
	}
	
	final int getHeight()
	{
		int max = 0;

		for (int i : getArray())
		{
			if (i > max)
			{
				max = i;
			}
		}
		return (int) (max * Y_SCALE);
	}
	
	final String getName()
	{
		return getClass().getSimpleName();
	}
	
	final void save() throws IOException
	{
		Graphics gfx = image.getGraphics();
		FileOutputStream os = null;
		try
		{
			os = new FileOutputStream(getName() + ".gif");
			int [] array = getArray();
			int yMax = getHeight();
			int x, s=0;
	
			gfx.setColor(Color.WHITE);
			gfx.fillRect(0, 0, image.getWidth(), image.getHeight());

			gfx.setColor(Color.LIGHT_GRAY);
			do
			{
				x = s * 16000000 / (256 * 1024);
				gfx.drawLine(
						X_OFFSET + (int)(x * X_SCALE), Y_OFFSET,
						X_OFFSET + (int)(x * X_SCALE), Y_OFFSET + yMax);
				s++;
			}
			while(x < getWidth());

			gfx.setColor(Color.BLACK);
			for (x = 1; x < array.length; x++)
			{
				gfx.drawLine(
						X_OFFSET + (int)((x-1) * X_SCALE),
						Y_OFFSET + (int)(yMax - array[x-1] * Y_SCALE),
						X_OFFSET + (int)(x * X_SCALE), 
						Y_OFFSET + (int)(yMax - array[x] * Y_SCALE));
			}
			ImageIO.write(image, "gif", os);
			os.flush();
		}
		finally
		{
			if (os != null)
			{
				os.close();
			}
			gfx.dispose();
		}
	}

	public abstract int[] getArray();
	
	public static void main(String [] args)
	{
		for (LightProfile profile : profiles)
		{
			try
			{
				profile.save();
			}
			catch (IOException e)
			{
				e.printStackTrace(System.err);
			}
		}
	}
}
