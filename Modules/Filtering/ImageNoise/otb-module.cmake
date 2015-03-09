set(DOCUMENTATION "This module is related to noise estimation and reduction.")

otb_module(OTBImageNoise
  DEPENDS
    OTBImageManipulation
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
