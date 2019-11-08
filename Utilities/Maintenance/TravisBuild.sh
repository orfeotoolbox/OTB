#!/usr/bin/env bash
#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#variables required
CTEST_COMMAND="/tmp/cmake-3.4.1-Linux-x86_64/bin/ctest"
CMAKE_MAKE_PROGRAM=/tmp/ninja
CTEST_CMAKE_GENERATOR="Ninja"
CTEST_DASHBOARD_ROOT=$HOME/build
WGET=`which wget`
SED=`which sed`

if [[ -z "$TRAVIS_BRANCH" ]]; then
    TRAVIS_BRANCH='develop'
    TRAVIS_BUILD_NUMBER=1
    TRAVIS_PULL_REQUEST='false'
    export TRAVIS_BRANCH
    export TRAVIS_BUILD_NUMBER
    export TRAVIS_PULL_REQUEST
fi

if [ ! -f $CTEST_DASHBOARD_ROOT/orfeotoolbox/OTB/CMakeLists.txt ]; then
    mkdir -p $CTEST_DASHBOARD_ROOT/orfeotoolbox
    cd $CTEST_DASHBOARD_ROOT/orfeotoolbox
    git clone --depth=1 --branch=$TRAVIS_BRANCH https://github.com/orfeotoolbox/OTB
fi

CMAKE_FILE=$CTEST_DASHBOARD_ROOT/orfeotoolbox/OTB/CMakeLists.txt
VERSION_MAJOR=$(sed -n  '/set.OTB_VERSION_MAJOR/p' $CMAKE_FILE|cut -d '"' -f 2)
VERSION_MINOR=$(sed -n  '/set.OTB_VERSION_MINOR/p' $CMAKE_FILE|cut -d '"' -f 2)
VERSION_PATCH=$(sed -n  '/set.OTB_VERSION_PATCH/p' $CMAKE_FILE|cut -d '"' -f 2)
OTB_VERSION="$VERSION_MAJOR.$VERSION_MINOR"
OTB_VERSION_FULL="$VERSION_MAJOR.$VERSION_MINOR.$VERSION_PATCH"

XDK_FILE=OTB-$OTB_VERSION_FULL-xdk-Linux64.run
CMAKE_PREFIX_PATH=/tmp/OTB-$OTB_VERSION_FULL-xdk-Linux64
OTB_INSTALL_DIR=$CTEST_DASHBOARD_ROOT/orfeotoolbox/install
XDK_INSTALL_DIR="/tmp/${XDK_FILE%.*}"

#export variables for later use in cmake file
export CTEST_COMMAND
export OTB_VERSION
export CTEST_DASHBOARD_ROOT
export CTEST_CMAKE_GENERATOR
export CMAKE_MAKE_PROGRAM
export CMAKE_PREFIX_PATH
export OTB_INSTALL_DIR
export XDK_INSTALL_DIR

#print
echo "TRAVIS_BRANCH=$TRAVIS_BRANCH"
echo "CTEST_COMMAND=$CTEST_COMMAND"
echo "OTB_VERSION=$OTB_VERSION"
echo "CTEST_DASHBOARD_ROOT=$CTEST_DASHBOARD_ROOT"
echo "XDK_FILE=$XDK_FILE"
echo "OTB_INSTALL_DIR=$OTB_INSTALL_DIR"
echo "CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH"
echo "XDK_INSTALL_DIR=$XDK_INSTALL_DIR"
echo "XDK_URL=https://www.orfeo-toolbox.org/packages/xdk/OTB-$OTB_VERSION/$XDK_FILE"

#change to /tmp
cd /tmp

#download, extract cmake binaries
wget https://cmake.org/files/v3.4/cmake-3.4.1-Linux-x86_64.tar.gz --no-check-certificate
tar -xzf cmake-3.4.1-Linux-x86_64.tar.gz

#download, extract ninja binaries
wget https://github.com/martine/ninja/releases/download/v1.6.0/ninja-linux.zip --no-check-certificate
unzip ninja-linux.zip

#download, extract OTB XDK
wget https://www.orfeo-toolbox.org/packages/xdk/OTB-$OTB_VERSION/$XDK_FILE --no-check-certificate
chmod +x /tmp/$XDK_FILE
/tmp/$XDK_FILE

echo "Running ctest -VV -S TravisBuild.cmake"
$CTEST_COMMAND -VV -S $CTEST_DASHBOARD_ROOT/orfeotoolbox/OTB/Utilities/Maintenance/TravisBuild.cmake
