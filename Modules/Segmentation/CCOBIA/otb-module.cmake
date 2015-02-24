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
    OTBITK

  TEST_DEPENDS
    OTBVectorDataBase
    OTBTestKernel
    OTBImageIO
    OTBVectorDataIO
    OTBSmoothing
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
