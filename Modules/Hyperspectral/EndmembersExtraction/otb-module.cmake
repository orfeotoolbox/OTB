set(DOCUMENTATION "This module contains classes related to he extraction of end
members (see also 'unmixing' module).")

otb_module(OTBEndmembersExtraction 
  DEPENDS
    OTBStatistics
    OTBProjection
    OTBImageBase
    OTBDimensionalityReduction
    OTBImageManipulation
    OTBBoost
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
