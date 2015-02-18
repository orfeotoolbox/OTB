set(DOCUMENTATION "Most of the fusion methods in the remote sensing community
deal with the pansharpening technique. This fusion combines the image from the
PANchromatic sensor of one satellite (high spatial resolution data) with the
multispectral (XS) data (lower resolution in several spectral bands) to generate images
with a high resolution and several spectral bands. This module provides classes
related to this purpose.")

otb_module(OTBPanSharpening 
  DEPENDS
    OTBImageBase
    OTBStatistics
    OTBImageManipulation
    OTBConvolution
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
