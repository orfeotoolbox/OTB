set(DOCUMENTATION "This module imports libsvm to the build system")

otb_module(OTBLibSVM
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable libSVM dependent modules" OFF)
