set(DOCUMENTATION "SAR Calibration application.")

otb_module(OTBAppSARCalibration 
  DEPENDS
    OTBSARCalibration
    OTBSAR
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
