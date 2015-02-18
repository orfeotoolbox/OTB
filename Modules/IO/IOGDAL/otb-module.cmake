set(DOCUMENTATION "This module contains features to read and write images with
GDAL.")

otb_module(OTBIOGDAL 
  DEPENDS
    OTBVectorDataBase
    OTBGdalAdapters
    OTBTinyXML
    OTBImageBase
    OTBCommon
    OTBGDAL
    OTBBoost
    OTBOSSIMAdapters
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
