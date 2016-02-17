set(DOCUMENTATION "This module imports OpenGL to the build system")

otb_module(OTBOpenGL
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable OpenGL dependent modules" OFF)
