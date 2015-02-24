set(DOCUMENTATION "Optical calibration application.")

otb_module(OTBAppOpticalCalibration
  DEPENDS
    OTBOpticalCalibration
    OTBImageManipulation
    OTBITK
    OTBApplicationEngine
    OTBProjection

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
