#!/bin/sh
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

cat_path()
{
  if [ $# -eq 0 ]; then exit 0; fi
  if [ $# -eq 1 ]; then echo "$1"; exit 0; fi
  cur="$1"
  shift 1
  next=$(cat_path "$@")
  if [ -z "$cur" ]; then
    echo "$next"
  elif [ -z "$next" ]; then
    echo "$cur"
  else
    echo "$cur:$next"
  fi
}

# The below environment variables only affect current shell
# So if you run again from a terminal, you need to run the script again
OS="$(lsb_release -is)"
# test the shell used before OS as variable used to resolve this script
# path may differ
if [ -n "${BASH}" ]; then
  # dirname does not exists on RH-based OS
  if [ $OS = "RedHatEnterprise" ] || [ $OS = "Fedora" ] || [ $OS = "RockyLinux" ]; then
    OTB_INSTALL_DIR="$(realpath $(dirname "${BASH_SOURCE[0]}"))"
  elif [ -n "${BASH}" ]; then
    OTB_INSTALL_DIR="$( dirname -- "$( readlink -f -- "${BASH_SOURCE[0]}"; )"; )"
  fi
else
  # non-bash shell
  OTB_INSTALL_DIR="$( dirname -- "$( readlink -f -- "$0"; )"; )"
fi
CMAKE_PREFIX_PATH=$OTB_INSTALL_DIR
export CMAKE_PREFIX_PATH

# check and set OTB_APPLICATION_PATH
OTB_APPLICATION_PATH=$(cat_path "$OTB_INSTALL_DIR/lib/otb/applications" "$OTB_APPLICATION_PATH")

# Add bin directory to system PATH
PATH=$(cat_path "$OTB_INSTALL_DIR/bin" "$PATH")

# export PYTHONPATH to import otbApplication.py
PYTHONPATH=$(cat_path "$OTB_INSTALL_DIR/lib/otb/python" "$PYTHONPATH")
if [ $OS = "RedHatEnterprise" ] || [ $OS = "Fedora" ] || [ $OS = "RockyLinux" ]; then
  PYTHONPATH=$(cat_path "$OTB_INSTALL_DIR/lib/python3.8/site-packages" "$PYTHONPATH")
else
  PYTHONPATH=$(cat_path "$OTB_INSTALL_DIR/lib/python3/dist-packages" "$PYTHONPATH")
fi

# set numeric locale to C
LC_NUMERIC=C

# set GDAL_DATA variable used by otb application
GDAL_DATA="$OTB_INSTALL_DIR/share/gdal"

PROJ_LIB="$OTB_INSTALL_DIR/share/proj"

export GDAL_DRIVER_PATH=disable

export LD_LIBRARY_PATH="$OTB_INSTALL_DIR/lib:$LD_LIBRARY_PATH"

# export variables
export LC_NUMERIC
export GDAL_DATA
export PROJ_LIB
export OTB_APPLICATION_PATH
export PATH
export PYTHONPATH
export OTB_INSTALL_DIR

# The first time after install, run that script to patch cmake files with the right install path. This cannot be done until OTB is installed
# This call permits to the user not to launch this by hand
if [ ! -e "$OTB_INSTALL_DIR"/tools/install_done.txt ]
then
  source "$OTB_INSTALL_DIR"/tools/post_install.sh
else
  echo "**** OTB environment setup complete ****"
fi
