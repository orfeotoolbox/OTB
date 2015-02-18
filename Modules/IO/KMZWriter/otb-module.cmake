set(DOCUMENTATION "This module contains features to write kmz files (supported
by Google Earth).")

otb_module(OTBKMZWriter 
  DEPENDS
    OTBVectorDataBase
    OTBImageIO
    OTBImageBase
    OTBTransform
    OTBOSSIMAdapters
    OTBImageManipulation
    OTBVectorDataIO
    OTBlibkml
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
