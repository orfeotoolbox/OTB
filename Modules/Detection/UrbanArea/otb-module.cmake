set(DOCUMENTATION "This module is concerned with the detection of urban area.")

otb_module(OTBUrbanArea 
  DEPENDS
    OTBIndices
    OTBImageBase
    OTBEdge
    OTBStatistics
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
