set(DOCUMENTATION "Markov Random Fields are probabilistic models that use the
statistical dependency between pixels in a neighborhood to infeer the value of a
given pixel. This module contains the OTB framework, considered as more flexible
than the ITK one.")

otb_module(OTBMarkov
  DEPENDS
    OTBBoost
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
