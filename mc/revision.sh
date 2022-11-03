#!/bin/bash

. ./version.sh

today=`date`
year=`date +%Y`
file=revision.h

echo "/*"  >$file
echo "**" >>$file
echo "**	Versions- und Revisionsnummer samt Log" >>$file
echo "**" >>$file
echo "**	Copyright (C) 2010-$year committers of this modelrailway project. All rights reserved." >>$file
echo "**" >>$file
echo "**	This program and the accompanying materials are made available under the" >>$file
echo "**	terms of the Eclipse Public License v1.0 and Eclipse Distribution License v. 1.0" >>$file
echo "**	which accompanies this distribution." >>$file
echo "**" >>$file
echo "**	The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html" >>$file
echo "**" >>$file
echo "**" >>$file
echo "*/" >>$file

echo ""  >>$file
echo "#ifndef REVISION_H" >>$file
echo "#define REVISION_H" >>$file
echo "" >>$file
echo "#define FIRMWARE_VERSION  ${MRW_VERSION}" >>$file
echo "#define FIRMWARE_REVISION ${MRW_REVISION}" >>$file
echo "" >>$file
echo "#endif" >>$file
echo "" >>$file

echo "/***********************************************************************" >>$file
git log . >>$file
echo "***********************************************************************/" >>$file

