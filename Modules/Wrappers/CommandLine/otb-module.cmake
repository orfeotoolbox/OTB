set(DOCUMENTATION "Required to launch applications from command line.")

otb_module(OTBCommandLine 
  DEPENDS
    OTBApplicationEngine
    OTBTinyXML
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
    OTBAppImageUtils
  DESCRIPTION
    "${DOCUMENTATION}"
  )
