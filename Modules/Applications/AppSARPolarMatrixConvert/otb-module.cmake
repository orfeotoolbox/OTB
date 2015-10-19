set(DOCUMENTATION "Basic filters application.")

otb_module(OTBAppSARPolarMatrixConvert
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
