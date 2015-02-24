set(DOCUMENTATION "Geometric moments (for pattern recognition) application.")

otb_module(OTBAppMoments
  DEPENDS
    OTBMoments
    OTBImageBase
    OTBApplicationEngine
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
