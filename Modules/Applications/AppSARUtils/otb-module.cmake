set(DOCUMENTATION "SAR Utils application.")

otb_module(OTBAppSARUtils 
  DEPENDS
    OTBMathParser
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
