set(DOCUMENTATION "This module is related to noise estimation and reduction.")

otb_module(OTBImageNoise 
  DEPENDS
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
