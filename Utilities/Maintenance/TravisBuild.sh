#!/usr/bin/env bash

#variables required
CTEST_COMMAND="/tmp/cmake-3.4.1-Linux-x86_64/bin/ctest"
CMAKE_MAKE_PROGRAM=/tmp/ninja
CTEST_CMAKE_GENERATOR="Ninja"
CTEST_DASHBOARD_ROOT=$HOME/build
WGET=`which wget`
SED=`which sed`

#develop xdk is used for all branches whose name does not contains release-X.Y
XDK_VERSION="develop"

if [[ -z "$TRAVIS_BRANCH" ]]; then
    TRAVIS_BRANCH='develop'
    TRAVIS_BUILD_NUMBER=1
    TRAVIS_PULL_REQUEST='false'
    export TRAVIS_BRANCH
    export TRAVIS_BUILD_NUMBER
    export TRAVIS_PULL_REQUEST
    mkdir -p $CTEST_DASHBOARD_ROOT/orfeotoolbox/build
    git clone --depth=1 --branch=$TRAVIS_BRANCH https://github.com/orfeotoolbox/OTB
fi

#if a branch contains release-X.Y then xdk is taken from packages/xdk/OTB-X.Y
if [[ "$TRAVIS_BRANCH" =~ release-* ]]; then
    VERSION_SMALL=$(echo $TRAVIS_BRANCH| sed 's/.*release-//'| cut -d'-' -f 1)
    XDK_VERSION="OTB-$VERSION_SMALL"
fi

VERSION_STRING=$($WGET -q -O- "https://www.orfeo-toolbox.org/packages/xdk/$XDK_VERSION/"| \
                        $SED -n 's,.*OTB-\([0-9][^<]*\).*-xdk-Linux.*,\1,p')

XDK_FILE=OTB-$VERSION_STRING-xdk-Linux64.run
CMAKE_PREFIX_PATH=/tmp/OTB-$VERSION_STRING-xdk-Linux64
OTB_INSTALL_DIR=$CTEST_DASHBOARD_ROOT/orfeotoolbox/install
XDK_INSTALL_DIR="/tmp/${XDK_FILE%.*}"

#export variables for later use in cmake file
export CTEST_COMMAND
export OTB_VERSION
export XDK_VERSION
export CTEST_DASHBOARD_ROOT
export CTEST_CMAKE_GENERATOR
export CMAKE_MAKE_PROGRAM
export CMAKE_PREFIX_PATH
export OTB_INSTALL_DIR
export XDK_INSTALL_DIR

#print
echo "VERSION_STRING=$VERSION_STRING"
echo "TRAVIS_BRANCH=$TRAVIS_BRANCH"
echo "CTEST_DASHBOARD_ROOT=$CTEST_DASHBOARD_ROOT"
echo "CTEST_COMMAND=$CTEST_COMMAND"
echo "XDK_VERSION=$XDK_VERSION"
echo "XDK_FILE=$XDK_FILE"
echo "OTB_INSTALL_DIR=$OTB_INSTALL_DIR"
echo "CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH"
echo "XDK_INSTALL_DIR=$XDK_INSTALL_DIR"

#change to /tmp
cd /tmp

#download, extract cmake binaries
wget https://cmake.org/files/v3.4/cmake-3.4.1-Linux-x86_64.tar.gz --no-check-certificate
tar -xzf cmake-3.4.1-Linux-x86_64.tar.gz

#download, extract ninja binaries
wget https://github.com/martine/ninja/releases/download/v1.6.0/ninja-linux.zip --no-check-certificate
unzip ninja-linux.zip

#download, extract OTB XDK
wget https://www.orfeo-toolbox.org/packages/xdk/$XDK_VERSION/$XDK_FILE --no-check-certificate
chmod +x /tmp/$XDK_FILE
/tmp/$XDK_FILE

echo "Running ctest -VV -S TravisBuild.cmake"
$CTEST_COMMAND -VV -S $CTEST_DASHBOARD_ROOT/orfeotoolbox/OTB/Utilities/Maintenance/TravisBuild.cmake
