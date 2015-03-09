set(DOCUMENTATION "This module provides Ossim Plugins")

otb_module(OTBOssimPlugins
  DEPENDS
    OTBGeoTIFF
    OTBGDAL
    OTBOssim
  TEST_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
  )
