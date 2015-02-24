set(DOCUMENTATION "Basic filters application.")

otb_module(OTBAppFiltering
  DEPENDS
    OTBImageNoise
    OTBImageManipulation
    OTBITK
    OTBApplicationEngine
    OTBImageBase

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
