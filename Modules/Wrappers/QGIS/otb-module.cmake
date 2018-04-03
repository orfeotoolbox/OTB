set(DOCUMENTATION "Required to generate descriptor files for QGIS processing plugin.")

otb_module(OTBQgis
  DEPENDS
    OTBITK
    OTBApplicationEngine
    
  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
otb_module_activation_option("Generating descriptors for QGIS processing" OFF)
