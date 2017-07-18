set(DOCUMENTATION "SAR Utils application.")

otb_module(OTBAppSARUtils
  DEPENDS
    OTBApplicationEngine
    OTBImageNoise
    OTBImageBase
    OTBITK
    
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
  )
