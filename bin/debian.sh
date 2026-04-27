#!/bin/bash

set -e

BASE=${PWD}
BUILD=${BASE}/rootfs
TARGET=${BASE}/target

PACKAGE=mrw-firmware
ARCH=all
. ${BASE}/mc/version.sh

rsync -av --delete rootfs/ ${TARGET}

mkdir -p ${TARGET}/lib/firmware/mrw/
mkdir -p ${TARGET}/usr/share/doc/${PACKAGE}/

cp -a ${BASE}/mc/firmware.hex ${TARGET}/lib/firmware/mrw/mrw-firmware-m32.hex
cp -a mc/version.txt ${TARGET}/usr/share/doc/${PACKAGE}/

COPYRIGHT=${TARGET}/usr/share/doc/${PACKAGE}/copyright
echo "Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/" > $COPYRIGHT
echo "Upstream-Name: $PACKAGE" >> ${COPYRIGHT}
echo "Source: https://github.com/stmork/mrw" >> ${COPYRIGHT}
echo "" >> ${COPYRIGHT}
echo "Files: *" >> ${COPYRIGHT}
echo "Copyright: 2018-`date +'%Y'` Steffen A. Mork <linux-dev@morknet.de>" >> ${COPYRIGHT}
echo "License: EPL-2.0" >> ${COPYRIGHT}
sed -e 's/^$/\./g' -e 's/^/ /g' LICENSE.md >> ${COPYRIGHT}

sed -i	-e "s/%ARCH%/${ARCH}/g"\
	-e "s/%VERSION%/${VERSION}/g"\
	-e "s/%BUILD%/${BUILD_NUMBER}/g"\
	-e "s/%PACKAGE%/${PACKAGE}/g"\
	${TARGET}/DEBIAN/control

echo "Packaging..."
rm -f ${BASE}/*.deb
chmod -R o+rX ${TARGET}
fakeroot dpkg -b ${TARGET} ${BASE}/${PACKAGE}_${VERSION}_${ARCH}.deb
