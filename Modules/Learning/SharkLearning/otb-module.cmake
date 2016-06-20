set(DOCUMENTATION "This module provides the Orfeo Toolbox machine learning tools based on Shark")

otb_module(OTBSharkLearning
  DEPENDS
    OTBApplicationEngine
    OTBBoost
    OTBColorMap
    OTBCommon
    OTBConversion
    OTBGdalAdapters
    OTBIOXML
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBInterpolation
    OTBMajorityVoting
    OTBObjectList
    OTBProjection
    OTBSampling
    OTBShark
    OTBStatistics
    OTBStreaming
    OTBSupervised
    OTBVectorDataBase
    OTBVectorDataIO
    OTBVectorDataManipulation


    TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase
    OTBBoost

  DESCRIPTION
    "${DOCUMENTATION}"
)
