set(DOCUMENTATION "This module contains features to write kmz files (supported
by Google Earth).")

otb_module(OTBKMZWriter
  DEPENDS
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBOSSIMAdapters
    OTBTransform
    OTBVectorDataBase
    OTBVectorDataIO
    OTBlibkml

  TEST_DEPENDS
    OTBCommandLineParser
    OTBProjection
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
