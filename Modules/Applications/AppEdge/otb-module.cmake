set(DOCUMENTATION "Edge detection and extraction application (see also Edge
module).")

otb_module(OTBAppEdge
  DEPENDS
    OTBStatistics
    OTBITK
    OTBImageBase
    OTBApplicationEngine
    OTBEdge
    OTBImageManipulation
    OTBProjection

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
