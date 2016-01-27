set(DOCUMENTATION "This module imports GLUT to the build system")

otb_module(OTBGlut
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable GLUT dependent modules" OFF)
