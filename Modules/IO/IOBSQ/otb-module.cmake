set(DOCUMENTATION "This module contains features to read BSQ format images.")

otb_module(OTBIOBSQ 
  DEPENDS
    OTBImageBase
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
