set(DOCUMENTATION "This module contains useful classes for object detection.")

otb_module(OTBObjectDetection 
  DEPENDS
    OTBVectorDataBase
    OTBTextures
    OTBStatistics
    OTBMoments
    OTBDescriptors
    OTBStreaming
    OTBCommon
    OTBImageBase
    OTBObjectList
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
    OTBIOXML
    OTBSVMLearning
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
