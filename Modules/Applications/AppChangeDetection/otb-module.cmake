set(DOCUMENTATION "Change detection application (see also ChangeDetection
module).")

otb_module(OTBAppChangeDetection 
  DEPENDS
    OTBApplicationEngine
    OTBChangeDetection
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
