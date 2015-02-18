set(DOCUMENTATION "This module contains features to read and write KML format
vector data.")

otb_module(OTBIOKML 
  DEPENDS
    OTBVectorDataBase
    OTBProjection
    OTBlibkml
    OTBOSSIMAdapters
    OTBCommon
    OTBGDAL
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
