set(DOCUMENTATION "This module imports Curl to the build system")

otb_module(OTBCurl
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable curl based functionnalities" ON)
