set(DOCUMENTATION "This module contains useful classes for object detection.")

otb_module(OTBObjectDetection
  DEPENDS
    OTBCommon
    OTBDescriptors
    OTBITK
    OTBImageBase
    OTBMoments
    OTBObjectList
    OTBStatistics
    OTBStreaming
    OTBTextures
    OTBVectorDataBase

  TEST_DEPENDS
    OTBIOXML
    OTBImageIO
    OTBSVMLearning
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
