set(DOCUMENTATION "This module contains features to read and write TileMap
format images.")

otb_module(OTBIOTileMap
  DEPENDS
    OTBIOGDAL
    OTBImageBase
    OTBCommon
    OTBCurlAdapters
    OTBITK

  TEST_DEPENDS
    OTBTransform
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation
    OTBProjection

  DESCRIPTION
    "${DOCUMENTATION}"
)
