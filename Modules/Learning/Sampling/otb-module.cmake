set(DOCUMENTATION "This module contains OTB sampling framework.")

otb_module(OTBSampling
  DEPENDS
    OTBCommon
    OTBImageManipulation
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
