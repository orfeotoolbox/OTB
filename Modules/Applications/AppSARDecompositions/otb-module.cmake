set(DOCUMENTATION "Basic filters application.")

otb_module(OTBAppSARDecompositions
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
