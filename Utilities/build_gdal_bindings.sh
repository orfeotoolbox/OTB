#!/bin/bash
# This script permits to build gdal bindings with python > 3.10 which is the default version in OTB 10

if [ "$#" -ne 3 ]; then
	echo "Usage : build_gdal_bindings.sh path_to_sources path_to_workdir otb_version"
	exit
fi

SOURCEDIR=$1
WORKDIR=$2
INSTALLDIR=$WORKDIR/gdal_bindings_otb-$3-py312

echo "*** Checking out branch release-$3 ***"
cd $SOURCEDIR
git checkout origin/release-$3

pyversion="$(python3 -V 2>&1)"
echo "*** Building GDAL bindings with $pyversion for otb $3 ***"
mkdir $WORKDIR/gdal_build_otb-$3 && cd $WORKDIR/gdal_build_otb-$3
cmake $SOURCEDIR/SuperBuild -DCMAKE_INSTALL_PREFIX=$INSTALLDIR -DDOWNLOAD_LOCATION=$WORKDIR/superbuild-archives
make -j16 GDAL
echo "*** Creating a tar gz of the gdal bindings in the directory $INSTALLDIR ***"
find $INSTALLDIR/lib/python3/dist-packages/osgeo/ -type f -name "*.so" -printf "%P\n" | tar -czf $INSTALLDIR/OTB-$3-GDAL-bindings-py312.tar.gz --no-recursion -C $INSTALLDIR/lib/python3/dist-packages/osgeo -T -
