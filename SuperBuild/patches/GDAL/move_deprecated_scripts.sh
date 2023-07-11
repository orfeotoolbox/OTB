#!/bin/sh -x
#
# Copyright (C) 2021 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

#Since gdal 3.2, some scripts were moved to osgeo/utils see https://github.com/OSGeo/gdal/blob/master/gdal/NEWS,
#section GDAL/OGR 3.2.0 Release Notes
#The installed scripts in bin are marked as deprecated
#we can no longer call them from there. So we remove them from superbuild install dir
SCRIPTS_DIR=$1

cd $SCRIPTS_DIR

for f in *.py;
do
if [ $f != "gdal2tiles.py" ]; then
    ln -s /usr/local/lib/python3.8/site-packages/osgeo_utils/$f $f 
fi
done