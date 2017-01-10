set(DOCUMENTATION "This module imports Glew to the build system")

otb_module(OTBGlew
  DEPENDS
  OTBOpenGL
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Glew dependent modules" OFF)
