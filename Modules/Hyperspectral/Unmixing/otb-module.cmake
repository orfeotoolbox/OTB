set(DOCUMENTATION "In hyperspectral imagery one pixel typically consists of a
mixture of the reflectance spectra of several materials, where the mixture
coefficients correspond to the abundances of the constituting materials. The process of
unmixing one of these 'mixed' pixels is called hyperspectral image unmixing or
simply hyperspectral unmixing. This module contains classes useful in meeting that
goal.")

otb_module(OTBUnmixing 
  DEPENDS
    OTBDimensionalityReduction
    OTBStatistics
    OTBBoost
    OTBCommon
    OTBWavelet
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
