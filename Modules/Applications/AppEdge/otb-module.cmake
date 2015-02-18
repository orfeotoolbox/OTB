set(DOCUMENTATION "Edge detection and extraction application (see also Edge
module).")

otb_module(OTBAppEdge 
  DEPENDS
    OTBStatistics
    OTBProjection
    OTBImageBase
    OTBEdge
    OTBApplicationEngine
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
