set(DOCUMENTATION "Optical calibration application.")

otb_module(OTBAppOpticalCalibration 
  DEPENDS
    OTBOpticalCalibration
    OTBProjection
    OTBApplicationEngine
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
