set(DOCUMENTATION "This module is concerned with texture analysis.
Co-occurrence matrix, Haralick are examples of what can be found in this module.")

otb_module(OTBTextures 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
