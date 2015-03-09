set(DOCUMENTATION "This module imports OpenCV to the build system")

otb_module(OTBOpenCV
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable OpenCV dependent modules" ON)
