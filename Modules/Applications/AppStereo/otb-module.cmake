set(DOCUMENTATION "Stereo application (see also Stereo module).")

otb_module(OTBAppStereo
  DEPENDS
    OTBApplicationEngine
    OTBDEM
    OTBDisparityMap
    OTBDisplacementField
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
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
