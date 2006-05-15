# $Id$

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(GDAL_INCLUDE_DIRS_BUILD_TREE 
  ${OTB_SOURCE_DIR}/Utilities/GDAL
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/raw
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libtiff
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/gtiff/libgeotiff
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg
  ${OTB_SOURCE_DIR}/Utilities/GDAL/frmts/msg/PublicDecompWT/COMP/WT/Inc
  ${OTB_SOURCE_DIR}/Utilities/GDAL/gcore
  ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr
  ${OTB_SOURCE_DIR}/Utilities/GDAL/ogr/ogrsf_frmts
  ${OTB_SOURCE_DIR}/Utilities/GDAL/port
  ${OTB_SOURCE_DIR}/Utilities/GDAL/alg
)

