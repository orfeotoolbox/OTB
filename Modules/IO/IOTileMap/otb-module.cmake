set(DOCUMENTATION "This module contains features to read and write TileMap
format images.")

otb_module(OTBIOTileMap 
  DEPENDS
    OTBIOGDAL
    OTBImageBase
    OTBCommon
    OTBCurlAdapters
  TEST_DEPENDS
    OTBTestKernel
    OTBProjection
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
