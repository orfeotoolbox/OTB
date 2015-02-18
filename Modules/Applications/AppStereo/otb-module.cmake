set(DOCUMENTATION "Stereo application (see also Stereo module).")

otb_module(OTBAppStereo 
  DEPENDS
    OTBStereo
    OTBStatistics
    OTBImageIO
    OTBImageBase
    OTBInterpolation
    OTBTransform
    OTBDisplacementField
    OTBDisparityMap
    OTBImageManipulation
    OTBMathParser
    OTBObjectList
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
