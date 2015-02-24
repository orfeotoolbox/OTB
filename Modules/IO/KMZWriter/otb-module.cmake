set(DOCUMENTATION "This module contains features to write kmz files (supported
by Google Earth).")

otb_module(OTBKMZWriter
  DEPENDS
    OTBVectorDataBase
    OTBImageIO
    OTBITK
    OTBlibkml
    OTBTransform
    OTBImageManipulation
    OTBImageBase
    OTBVectorDataIO
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBTestKernel
    OTBCommon
    OTBProjection

  DESCRIPTION
    "${DOCUMENTATION}"
)
