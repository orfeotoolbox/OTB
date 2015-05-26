set(DOCUMENTATION "This module imports MuParserX to the build system")

otb_module(OTBMuParserX
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable MuParserX dependent modules" OFF)
