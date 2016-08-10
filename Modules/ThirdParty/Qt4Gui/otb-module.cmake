set(DOCUMENTATION "This module imports QtGui to the build system")

otb_module(OTBQt4Gui
  DEPENDS
    OTBQt4Base
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Qt4 dependent modules" OFF)
