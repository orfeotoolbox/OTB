set(DOCUMENTATION "This module contains features to read LUM format images.")

otb_module(OTBIOLUM 
  DEPENDS
    OTBImageBase
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
