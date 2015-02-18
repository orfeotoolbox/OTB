set(DOCUMENTATION "Adapters for the Gdal Library ( for reading and writing  ).")

otb_module(OTBGdalAdapters 
  DEPENDS
    OTBBoostAdapters
    OTBBoost
    OTBCommon
    OTBGDAL
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
