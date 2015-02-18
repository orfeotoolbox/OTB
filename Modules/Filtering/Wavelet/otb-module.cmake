set(DOCUMENTATION "This module contains classes for wavelet analysis.")

otb_module(OTBWavelet 
  DEPENDS
    OTBObjectList
    OTBCommon
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
