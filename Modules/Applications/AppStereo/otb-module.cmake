set(DOCUMENTATION "Stereo application (see also Stereo module).")

otb_module(OTBAppStereo
  DEPENDS
    OTBApplicationEngine
    OTBDEM
    OTBDisparityMap
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBInterpolation
    OTBMathParser
    OTBObjectList
    OTBStatistics
    OTBStereo
    OTBTransform

  TEST_DEPENDS
    OTBCommandLine
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
