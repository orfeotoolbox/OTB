set(DOCUMENTATION "This module imports GLFW to the build system")

otb_module(OTBGLFW
  DEPENDS
  OTBOpenGL
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable GLFW dependent modules" OFF)
