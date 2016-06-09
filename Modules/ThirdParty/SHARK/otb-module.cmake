set(DOCUMENTATION "This module imports SHARK to the build system")

otb_module(OTBSHARK
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable SHARK dependent modules" OFF)
