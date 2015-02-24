set(DOCUMENTATION "Required to launch applications from command line.")

otb_module(OTBCommandLine
  DEPENDS
    OTBCommon
    OTBITK
    OTBTinyXML
    OTBApplicationEngine

  TEST_DEPENDS
    OTBTestKernel
    OTBAppImageUtils

  DESCRIPTION
    "${DOCUMENTATION}"
)
