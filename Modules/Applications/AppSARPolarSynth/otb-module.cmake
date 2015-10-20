set(DOCUMENTATION "Basic filters application.")

otb_module(OTBAppSARPolarSynth
  DEPENDS
    OTBPolarimetry
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
