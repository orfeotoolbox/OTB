set(DOCUMENTATION "Test application.")

otb_module(OTBAppTest 
  DEPENDS
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
