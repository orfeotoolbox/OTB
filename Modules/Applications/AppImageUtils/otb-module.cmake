set(DOCUMENTATION "Image utils and handling application.")

otb_module(OTBAppImageUtils
  DEPENDS
    OTBStatistics
    OTBColorMap
    OTBImageBase
    OTBTransform
    OTBKMZWriter
    OTBImageManipulation
    OTBStreaming
    OTBCarto
    OTBApplicationEngine
    OTBCurlAdapters
    OTBITK
    OTBMathParser
    OTBCommon
    OTBObjectList
    OTBOSSIMAdapters
    OTBProjection

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
