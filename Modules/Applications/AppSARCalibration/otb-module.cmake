set(DOCUMENTATION "SAR Calibration application.")

otb_module(OTBAppSARCalibration 
  DEPENDS
    OTBSARCalibration
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
