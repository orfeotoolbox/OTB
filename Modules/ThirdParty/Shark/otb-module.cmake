set(DOCUMENTATION "This module imports SHARK to the build system")

otb_module(OTBShark
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable SHARK dependent modules" OFF)

otb_module_requires_cxx11()

