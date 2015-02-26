set(DOCUMENTATION "Geometric moments are widely used in image processing and
pattern recognition. This module contains classes that implement many moments: Hu,
Flusser, and so on. Note that some of them can be evaluated along a path, instead
of an image.")

otb_module(OTBMoments
  DEPENDS
    OTBCommon
    OTBITK
    OTBPath

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBInterpolation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
