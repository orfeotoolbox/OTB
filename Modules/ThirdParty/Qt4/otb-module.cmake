set(DOCUMENTATION "This module imports Qt4 to the build system")

otb_module(OTBQt4
  DEPENDS
    OTBQt4OpenGL
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Qt4 dependent modules" OFF)
