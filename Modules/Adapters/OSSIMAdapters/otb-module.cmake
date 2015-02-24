set(DOCUMENTATION "Adapters for the OSSIM library (geospatial library used to
process imagery, maps, terrain, and vector data). It is used inside OTB for
several purposes: sensor models (especially RPC), metadata parsers, DEM handlers, and
so on.")

otb_module(OTBOSSIMAdapters
  DEPENDS
    OTBOpenThreads
    OTBITK
    OTBCommon
    OTBGDAL
    OTBOssimPlugins
    OTBOssim

  TEST_DEPENDS
    OTBTestKernel
    OTBImageBase
    OTBImageIO
    OTBTransform
    OTBProjection

  DESCRIPTION
    "${DOCUMENTATION}"
)
