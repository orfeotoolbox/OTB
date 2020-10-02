#!/bin/sh
#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
# So if you run again from a terminal. you need to run the script again
# see how this is sourced in monteverdi.sh and mapla.sh

CMAKE_PREFIX_PATH=OUT_DIR
export CMAKE_PREFIX_PATH

# check and set OTB_APPLICATION_PATH
OTB_APPLICATION_PATH=$(cat_path "OUT_DIR/lib/otb/applications" "$OTB_APPLICATION_PATH")

# Add bin direcotory to system PATH
PATH=$(cat_path "OUT_DIR/bin" "$PATH")

# export PYTHONPATH to import otbApplication.py
PYTHONPATH=$(cat_path "OUT_DIR/lib/python" "$PYTHONPATH")

# set numeric locale to C
LC_NUMERIC=C

# set GDAL_DATA variable used by otb application
GDAL_DATA=OUT_DIR/share/gdal

PROJ_LIB=OUT_DIR/share/proj

export GDAL_DRIVER_PATH=disable

# export variables
export LC_NUMERIC
export GDAL_DATA
export PROJ_LIB
export OTB_APPLICATION_PATH
export PATH
export PYTHONPATH

