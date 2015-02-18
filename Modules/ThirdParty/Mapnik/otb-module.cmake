set(DOCUMENTATION "This module imports Mapnik to the build system")

otb_module(OTBMapnik
  DEPENDS
    
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )

otb_module_activation_option("Enable Mapnik dependent modules" OFF)
