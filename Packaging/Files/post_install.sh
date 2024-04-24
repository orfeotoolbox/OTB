#!/bin/sh
#
# Copyright (C) 2005-2023 Centre National d'Etudes Spatiales (CNES)
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
echo "***** First time launching OTB after installation, doing some post installation steps before use *****"
# Apply necessary patches for a modular install because cmake generates these file at configure time, not at packaging time
sed -i "s/FATAL_ERROR/WARNING/g" "$OTB_INSTALL_DIR"/lib/cmake/OTB-9.0/OTBTargets.cmake
sed -i "s/FATAL_ERROR/WARNING/g" "$OTB_INSTALL_DIR"/lib/cmake/OTB-9.0/OTBModuleAPI.cmake
sed -i "s/\/builds\/otb\/xdk/\${OTB_INSTALL_PREFIX}/g" "$OTB_INSTALL_DIR"/lib/cmake/OTB-9.0/*.cmake
sed -i "s/\/builds\/otb\/xdk/\${OTB_INSTALL_PREFIX}/g" "$OTB_INSTALL_DIR"/lib/cmake/OTB-9.0/Modules/*.cmake
sed -i "s/\/builds\/otb\/xdk/\${OTB_INSTALL_PREFIX}/g" "$OTB_INSTALL_DIR"/lib/cmake/ITK-4.13/*.cmake
sed -i "s/\/builds\/otb\/xdk/\${OTB_INSTALL_PREFIX}/g" "$OTB_INSTALL_DIR"/lib/cmake/ITK-4.13/Modules/*.cmake
sed -i "s/\/builds\/otb\/xdk/\$OTB_INSTALL_DIR/g" "$OTB_INSTALL_DIR"/bin/gdal-config
sed -i "s/\/builds\/otb\/xdk/\$OTB_INSTALL_DIR/g" "$OTB_INSTALL_DIR"/bin/curl-config
sh "$OTB_INSTALL_DIR"/tools/sanitize_rpath.sh
# Check python version, if python 3.10 (ubuntu 22 and debian 12) download and extract the gdal bindings for python 3.10
pyversion="$(python3 -V 2>&1 | sed 's/.* \([0-9]\).\([0-9]\).*/\1\2/')"
if [ "$pyversion" = "31" ]; then
    echo "*** Python 3.10 detected, downloading gdal bindings compiled for python 3.10 ***"
    curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-GDAL-bindings-py310.tar.gz -o "$OTB_INSTALL_DIR"/tools/py310.tar.gz
    tar -xf "$OTB_INSTALL_DIR"/tools/py310.tar.gz -C "$OTB_INSTALL_DIR"/lib/python3/dist-packages/osgeo/
    rm "$OTB_INSTALL_DIR"/tools/py310.tar.gz
    echo "*** GDAL bindings for python 3.10 successfully installed ***"
fi
echo "OK" > "$OTB_INSTALL_DIR"/tools/install_done.txt
