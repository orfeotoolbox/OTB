set(DOCUMENTATION "This module imports libkml to the build system")

otb_module(OTBlibkml
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable libkml dependent modules" OFF)
