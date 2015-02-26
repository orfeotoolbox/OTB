set(DOCUMENTATION "This module contains classes that implement the Connected
Component algorithm (segmentation).")

otb_module(OTBCCOBIA
  DEPENDS
    OTBCommon
    OTBConversion
    OTBITK
    OTBLabelMap
    OTBMathParser
    OTBProjection
    OTBStreaming

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBSmoothing
    OTBTestKernel
    OTBVectorDataBase
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
