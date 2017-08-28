#!/bin/sh
#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

# The below environment variables only affect current shell
# So if you run again from a terminal. you need to run the script again
# see how this is sourced in monteverdi.sh and mapla.sh

# unset any existing LD_LIBRARY_PATH
unset LD_LIBRARY_PATH

CMAKE_PREFIX_PATH=OUT_DIR
export CMAKE_PREFIX_PATH


# if OTB_USE_LOCAL_GTK is set to one,
# we must include ./lib/gtk because gtklibs are installed there. 
# OTB_USE_LOCAL_GTK is not set by default (use GTK system)
#This code only affect linux system. for osx OUT_DIR/lib/gtk does not exists
if [ "$OTB_USE_LOCAL_GTK" = "1" ]; then
    if [ -d "OUT_DIR/lib/gtk" ]; then
	LD_LIBRARY_PATH=OUT_DIR/lib/gtk
	export LD_LIBRARY_PATH
    fi
fi

# check and set OTB_APPLICATION_PATH
if [ -z "$OTB_APPLICATION_PATH" ] || [ "$OTB_APPLICATION_PATH" = "" ]; then
    OTB_APPLICATION_PATH=OUT_DIR/lib/otb/applications
else
    OTB_APPLICATION_PATH=OUT_DIR/lib/otb/applications:$OTB_APPLICATION_PATH
fi

# Add bin direcotory to system PATH
PATH=OUT_DIR/bin:$PATH

# export PYTHONPATH to import otbApplication.py
PYTHONPATH=OUT_DIR/lib/python:$PYTHONPATH

# set numeric locale to C
LC_NUMERIC=C

# set GDAL_DATA variable used by otb application
GDAL_DATA=OUT_DIR/share/gdal

export GDAL_DRIVER_PATH=disable

# set GEOTIFF_CSV variable used by otb application
GEOTIFF_CSV=OUT_DIR/share/epsg_csv

# export variables
export LC_NUMERIC
export GDAL_DATA
export GEOTIFF_CSV
export OTB_APPLICATION_PATH
export PATH
export PYTHONPATH

