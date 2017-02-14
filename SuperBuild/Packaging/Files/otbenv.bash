#!/usr/bin/env bash

CURRENT_DIR=$(cd -P -- "$(dirname -- "$BASH_SOURCE")" && printf '%s\n' "$(pwd -P)")

unset LD_LIBRARY_PATH

PATH=$CURRENT_DIR/bin:$PATH
GDAL_DATA=$CURRENT_DIR/share/gdal
GEOTIFF_CSV=$CURRENT_DIR/share/epsg_csv

export PATH
export GDAL_DATA
export GEOTIFF_CSV
