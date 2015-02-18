set(DOCUMENTATION "Fusion of classifications application.")

otb_module(OTBAppFusion 
  DEPENDS
    OTBImageBase
    OTBInterpolation
    OTBPanSharpening
    OTBApplicationEngine
    OTBProjection
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
