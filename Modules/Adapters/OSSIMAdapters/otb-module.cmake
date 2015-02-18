set(DOCUMENTATION "Adapters for the OSSIM library (geospatial library used to
process imagery, maps, terrain, and vector data). It is used inside OTB for
several purposes: sensor models (especially RPC), metadata parsers, DEM handlers, and
so on.")

otb_module(OTBOSSIMAdapters 
  DEPENDS
    OTBGDAL
    OTBCommon
    OTBOssim
    OTBOssimPlugins
    OTBOpenThreads
  TEST_DEPENDS
    OTBTestKernel
    OTBProjection
    OTBSimulation
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
