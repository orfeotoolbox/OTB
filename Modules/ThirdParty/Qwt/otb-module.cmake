set(DOCUMENTATION "This module imports Qwt to the build system")

otb_module(OTBQwt
  DEPENDS
    OTBQt4
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Qwt dependent modules" OFF)
