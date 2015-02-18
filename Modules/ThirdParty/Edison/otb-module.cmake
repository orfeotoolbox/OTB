set(DOCUMENTATION "This module imports Edison to the build system")

otb_module(OTBEdison
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Edison segmentation dependent modules" ON)
