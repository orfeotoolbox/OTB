set(DOCUMENTATION "This module contains features to read and write TileMap
format images.")

otb_module(OTBIOTileMap
ENABLE_SHARED
  DEPENDS
    OTBITK
    OTBImageBase
    OTBIOGDAL
    OTBCommon
    OTBCurlAdapters
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBTestKernel
    OTBProjection
    OTBImageIO
    OTBTransform
    OTBImageManipulation

  DESCRIPTION
    "${DOCUMENTATION}"
)
