set(DOCUMENTATION "This module contains OTB sampling framework.")

otb_module(OTBSampling
  DEPENDS
    OTBCommon
    OTBConversion
    OTBImageManipulation
    OTBITK
    OTBStatistics

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase
    OTBIOXML

  DESCRIPTION
    "${DOCUMENTATION}"
)
