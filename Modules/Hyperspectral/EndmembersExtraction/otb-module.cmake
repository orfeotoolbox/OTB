set(DOCUMENTATION "This module contains classes related to he extraction of end
members (see also 'unmixing' module).")

otb_module(OTBEndmembersExtraction
  DEPENDS
    OTBStatistics
    OTBITK
    OTBImageBase
    OTBDimensionalityReduction
    OTBImageManipulation
    OTBProjection
    OTBBoostAdapters
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
