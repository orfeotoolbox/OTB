set(DOCUMENTATION "Classification application.")

otb_module(OTBAppClassification
  DEPENDS
    OTBVectorDataBase
    OTBConversion
    OTBStatistics
    OTBColorMap
    OTBBoost
    OTBInterpolation
    OTBMajorityVoting
    OTBVectorDataIO
    OTBSOM
    OTBSupervised
    OTBApplicationEngine
    OTBIndices
    OTBMathParser
    OTBITK
    OTBDempsterShafer
    OTBGdalAdapters
    OTBProjection
    OTBImageBase
    OTBIOXML
    OTBVectorDataManipulation
    OTBStreaming
    OTBImageManipulation
    OTBObjectList
    OTBCommon
    OTBSampling

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
