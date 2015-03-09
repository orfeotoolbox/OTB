set(DOCUMENTATION "This module imports MuParser to the build system")

otb_module(OTBMuParser
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable MuParser dependent modules" ON)
