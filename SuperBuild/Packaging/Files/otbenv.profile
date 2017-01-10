#!/bin/sh

DIRNAME_0=$(dirname "$0")
cd  "$DIRNAME_0"

CURRENT_DIR=$(pwd)

unset LD_LIBRARY_PATH

PATH=$PATH:$CURRENT_DIR/bin
GDAL_DATA=$CURRENT_DIR/share/gdal
GEOTIFF_CSV=$CURRENT_DIR/share/epsg_csv

export PATH
export GDAL_DATA
export GEOTIFF_CSV

