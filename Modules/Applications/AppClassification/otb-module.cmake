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
    OTBSVMLearning
    OTBLearningBase
    OTBApplicationEngine
    OTBIndices
    OTBMathParser
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
    OTBStreaming
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
