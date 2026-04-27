//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2026 Steffen A. Mork
//

pipeline
{
	agent any

	stages
	{
		stage ('Generate light profiles')
		{
			steps
			{
				dir ('Lichtprofile')
				{
					sh 'ant clean run'
				}
			}
		}
		stage ('Build Firmware')
		{
			steps
			{
				dir ('mc')
				{
					sh """
					make clean depend
					make
					"""
				}
			}
		}
		stage ('Build CAN-Prog')
		{
			steps
			{
				dir ('canprog')
				{
					sh """
					make clean depend
					make
					"""
				}
			}
		}
		stage ('Build Tools')
		{
			steps
			{
				dir ('tools')
				{
					sh """
					make clean depend
					make
					"""
				}
			}
		}
		stage ('Build Daemon')
		{
			steps
			{
				dir ('daemon')
				{
					sh """
					make clean depend
					make
					"""
				}
			}
		}
		stage ('Package')
		{
			steps
			{
				sh 'bin/debian.sh'
			}
		}
	}

	post
	{
		success
		{
			archiveArtifacts artifacts: '*.deb,*/*.hex', followSymlinks: false
		}
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'info@eisenbahnsteuerung.org'])
		}
	}
}
