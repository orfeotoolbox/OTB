set(DOCUMENTATION "This module contains features to read and write images with
GDAL.")

otb_module(OTBIOGDAL
  DEPENDS
    OTBVectorDataBase
    OTBGdalAdapters
    OTBITK
    OTBImageBase
    OTBCommon
    OTBTinyXML
    OTBGDAL
    OTBBoostAdapters
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
