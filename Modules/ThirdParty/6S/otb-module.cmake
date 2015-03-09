set(DOCUMENTATION "This module imports 6S to the build system")

otb_module(OTB6S
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable 6S radiometric and atmospheric correction module" ON)
