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

function Print-Usage {
  echo "Usage: $0 <compiler_arch>  <project>  [<target-os>  [<vc_version>]]"
  echo "  <compiler_arch> : 'x86' | 'x64'"
  echo "  <project>       : 'xdk' | 'otb'"
  echo "  <target-os>     : '8.1' | '10' (default)"
  echo "  <vc_version>    :"
  echo "    '14.20' (i.e. VS 2019)"
  echo "    '14.16' (i.e. VS 2017)"
  echo "    '14.0'  (i.e. VS 2015) (default)"
}
# check input arguments

if ( $args.count -lt 1 ) {
  echo "No arch"
  Print-Usage
  Break
}

if ( $args[0] -match "help") {
  Print-Usage
  Break
}

if ( $args.count -lt 2 ) {
  echo "No project"
  Print-Usage
  Break
}


$ARCH=$args[0]
$PROJECT=$args[1]

if ( $args.count -lt 3 ) {
  $SHORT_TARGET=10
} else {
  $SHORT_TARGET=$args[2]
}

if ( $args.count -lt 4 ) {
  $VCVER=14.29
} else {
  $VCVER=$args[3]
}

$Global:TARGET=$SHORT_TARGET
if ( $SHORT_TARGET -eq 10 ) {
  $Global:TARGET=10.0.17763.0
}

# Setup home dir (so that ssh configuration works fine)
# if "%USERNAME%"=="otbbot" (
$Global:HOMEDRIVE="C:"
$Global:HOMEPATH="\Users\otbbot"
#)
echo "Home dir: $HOMEDRIVE$HOMEPATH"

# Get the folder of current script
$SCRIPT_DIR=Split-Path $MyInvocation.MyCommand.Path -Parent

# Setup Python
. "$SCRIPT_DIR\setup_python.ps1" $ARCH

# Setup GL dlls
$env:PATH="$env:PATH;C:\tools\GL\$ARCH\bin"

# Setup compiler
# Thanks to Microsoft stupidness for not providing a script to set Development
# toolset in Powershell, they advice to use a third party script
# able to take vars from cmd and init them in powershell
# see official M$ answer saying they wont do the job:
# https://developercommunity.visualstudio.com/t/Provide-a-PowerShell-version-of-vcvarsal/10238319


. "$SCRIPT_DIR\Invoke-CmdScript.ps1"

Invoke-CmdScript "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" "$ARCH $TARGET -vcvars_ver=$VCVER"

# Setup Clcache
$Global:BUILDCACHE_DIR="C:\buildcache\$PROJECT-$ARCH-$TARGET-$VCVER"

if ( $PROJECT -eq "xdk" ) {
  $Global:BUILDCACHE_MAX_CACHE_SIZE=3000000000
}
if ( $PROJECT -eq "otb" ) {
  $Global:BUILDCACHE_MAX_CACHE_SIZE=2000000000
}
$env:PATH="C:\tools\buildcache\bin;$env:PATH"

# define image name as env variable as it is used in prepare_superbuild.cmake
$env:IMAGE_NAME="windows-$SHORT_TARGET-$ARCH-vc$VCVER"
echo "Generated IMAGE_NAME: $env:IMAGE_NAME"

# setup path to perl, but add it last ... (there is a libstdc++.dll in that folder...)
$env:PATH="$env:PATH;C:\tools\perl\perl\bin"
