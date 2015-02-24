set(DOCUMENTATION "This module contains classes for wavelet analysis.")

otb_module(OTBWavelet
  DEPENDS
    OTBImageManipulation
    OTBObjectList
    OTBITK
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
