#!/bin/bash
#
# Copyright (C) 2005-2025 Centre National d'Etudes Spatiales (CNES)
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

#Initialize variables if no input parameters has been set
OTB_VERSION="9.1.0"
INSTALL_FOLDER="$HOME/otb-$OTB_VERSION"

if [ ! -z "$1" ]:
    OTB_VERSION="$1"
fi

if [ ! -z "$2" ]:
    INSTALL_FOLDER="$2"
fi

ostype="$(lsb_release -is)"
LINUX_TYPE="Linux"
if [ "$ostype" == "RedHatEnterprise" ] ; then
    LINUX_TYPE="Linux_RedHat"
fi
echo "This script will download OTB $OTB_VERSION and install it to $INSTALL_FOLDER"
# Download and install
curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-$OTB_VERSION-$LINUX_TYPE.tar.gz -o /tmp/OTB-$OTB_VERSION-Linux.tar.gz
tar xf /tmp/OTB-$OTB_VERSION-Linux.tar.gz --one-top-level=$INSTALL_FOLDER
# Source the environment
. $INSTALL_FOLDER/otbenv.profile
cmakepresent="$(which cmake)"
swigpresent="$(which swig)"
if [ "$cmakepresent" != "" ] && [ "$swigpresent" != "" ]; then
        concatpyversion="$(python3 -V 2>&1 | sed 's/.* \([0-9]\).\([0-9]*\).*/\1\2/')"
        if [ "$concatpyversion" -ne "38" ]; then
                echo "*** Recompiling OTB Python bindings to be ready to use with your Python version ***"
                ctest -S "$INSTALL_FOLDER/share/otb/swig/build_wrapping.cmake"
        fi
fi

echo -e "\nOTB is ready to be used, in a terminal type : \n\nsource $INSTALL_FOLDER/otbenv.profile\notbcli_AppName to launch an otb application"