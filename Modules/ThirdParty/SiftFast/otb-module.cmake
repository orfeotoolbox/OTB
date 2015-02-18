set(DOCUMENTATION "This module imports SiftFast to the build system")

otb_module(OTBSiftFast
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable SiftFast dependent modules" ON)
