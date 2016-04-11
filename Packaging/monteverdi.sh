#!/bin/sh

cd `dirname $0`

THIS_DIR=`pwd`

export OTB_APPLICATION_PATH=$THIS_DIR/lib/otb/applications

export GDAL_DATA=$THIS_DIR/share/gdal

export GEOTIFF_CSV=$THIS_DIR/share/epsg_csv

cd $THIS_DIR/bin

$THIS_DIR/bin/monteverdi "$@"
