set(DOCUMENTATION "This module imports the OpenGL Qt4 components to the build system")

otb_module(OTBQt4OpenGL
  DEPENDS
    OTBQt4Base
    OTBQt4Gui
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Qt4 dependent modules" OFF)
