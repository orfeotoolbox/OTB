set(DOCUMENTATION "This module contains classes for wavelet analysis.")

otb_module(OTBWavelet
  DEPENDS
    OTBCommon
    OTBITK
    OTBObjectList

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
