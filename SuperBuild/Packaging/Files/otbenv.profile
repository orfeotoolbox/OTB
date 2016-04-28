#!/bin/sh

cd `dirname $0`

CURRENT_DIR=`pwd`

PATH=$PATH:$CURRENT_DIR/bin
GDAL_DATA=$CURRENT_DIR/share/gdal
GEOTIFF_CSV=$CURRENT_DIR/share/espg_csv

export PATH
export GDAL_DATA
export GEOTIFF_CSV
