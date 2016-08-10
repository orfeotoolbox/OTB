set(DOCUMENTATION "This module imports the base Qt4 components to the build system")

otb_module(OTBQt4Base
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Qt4 dependent modules" OFF)
