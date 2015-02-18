set(DOCUMENTATION "Image utils and handling application.")

otb_module(OTBAppImageUtils 
  DEPENDS
    OTBStatistics
    OTBColorMap
    OTBImageBase
    OTBTransform
    OTBApplicationEngine
    OTBStreaming
    OTBCarto
    OTBImageManipulation
    OTBCurlAdapters
    OTBProjection
    OTBMathParser
    OTBCommon
    OTBInterpolation
    OTBKMZWriter
    OTBOSSIMAdapters
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
