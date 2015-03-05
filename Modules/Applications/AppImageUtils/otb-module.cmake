set(DOCUMENTATION "Image utils and handling application.")

otb_module(OTBAppImageUtils
  DEPENDS
    OTBApplicationEngine
    OTBCarto
    OTBColorMap
    OTBCommon
    OTBCurlAdapters
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBMathParser
    OTBMathParserX
    OTBOSSIMAdapters
    OTBObjectList
    OTBProjection
    OTBStatistics
    OTBStreaming
    OTBTransform

  TEST_DEPENDS
    OTBCommandLine
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
