set(DOCUMENTATION "This module contains useful classes for object detection.")

otb_module(OTBObjectDetection
  DEPENDS
    OTBVectorDataBase
    OTBTextures
    OTBStatistics
    OTBITK
    OTBMoments
    OTBDescriptors
    OTBStreaming
    OTBImageManipulation
    OTBImageBase
    OTBObjectList
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBIOXML
    OTBImageIO
    OTBSVMLearning
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
