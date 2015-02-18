set(DOCUMENTATION "This module contains classes that implement the Connected
Component algorithm (segmentation).")

otb_module(OTBCCOBIA 
  DEPENDS
    OTBConversion
    OTBStatistics
    OTBProjection
    OTBMoments
    OTBLabelMap
    OTBStreaming
    OTBCommon
    OTBMathParser
  TEST_DEPENDS
    OTBTestKernel
    OTBSmoothing
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
