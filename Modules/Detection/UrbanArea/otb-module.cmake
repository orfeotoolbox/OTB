set(DOCUMENTATION "This module is concerned with the detection of urban area.")

otb_module(OTBUrbanArea
  DEPENDS
    OTBStatistics
    OTBITK
    OTBImageBase
    OTBEdge
    OTBImageManipulation
    OTBIndices

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
