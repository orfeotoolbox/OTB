#!/bin/bash
#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This script permits to build gdal bindings with python > 3.10 which is the default version in OTB 10

if [ "$#" -ne 3 ]; then
	echo "Usage : build_gdal_bindings.sh path_to_sources path_to_workdir otb_version"
	exit
fi

SOURCEDIR="$1"
WORKDIR="$2"
concatpyversion="$(python3 -V 2>&1 | sed 's/.* \([0-9]\).\([0-9]*\).*/\1\2/')"
INSTALLDIR="$WORKDIR/gdal_bindings_otb-$3-py$concatpyversion"

echo "*** Checking out branch release-$3 ***"
cd "$SOURCEDIR"
git checkout origin/release-$3

echo "*** Building GDAL bindings with $concatpyversion for otb $3 ***"
mkdir "$WORKDIR/gdal_build_otb-$3" && cd "$WORKDIR/gdal_build_otb-$3"
cmake "$SOURCEDIR/SuperBuild" -DCMAKE_INSTALL_PREFIX="$INSTALLDIR" -DDOWNLOAD_LOCATION="$WORKDIR/superbuild-archives"
make -j16 GDAL

echo "*** Creating a tar gz of the gdal bindings in the directory $INSTALLDIR ***"
osgeopython=$(find "$INSTALLDIR/lib" -mindepth 3 -maxdepth 3 -type d -name "osgeo")
find "$osgeopython" -type f -name "*.so" -printf "%P\n" | tar -czf "$INSTALLDIR/OTB-$3-GDAL-bindings-py$concatpyversion.tar.gz" --no-recursion -C "$osgeopython" -T -
