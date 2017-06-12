#!/usr/bin/env bash

CURRENT_DIR=$(cd -P -- "$(dirname -- "$BASH_SOURCE")" && printf '%s\n' "$(pwd -P)")

unset LD_LIBRARY_PATH

PATH=$CURRENT_DIR/bin:$PATH
GDAL_DATA=$CURRENT_DIR/share/data
GEOTIFF_CSV=$CURRENT_DIR/share/epsg_csv
PYTHONPATH=$CURRENT_DIR/lib/python:$PYTHONPATH
OTB_APPLICATION_PATH=$CURRENT_DIR/lib/otb/applications

LC_NUMERIC=C

export PATH
export GDAL_DATA
export GEOTIFF_CSV
export PYTHONPATH
export OTB_APPLICATION_PATH
export LC_NUMERIC
