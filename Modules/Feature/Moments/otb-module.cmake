set(DOCUMENTATION "Geometric moments are widely used in image processing and
pattern recognition. This module contains classes that implement many moments: Hu,
Flusser, and so on. Note that some of them can be evaluated along a path, instead
of an image.")

otb_module(OTBMoments
  DEPENDS
    OTBVectorDataBase
    OTBLabelMap
    OTBStatistics
    OTBITK
    OTBImageBase
    OTBVectorDataManipulation
    OTBCommon
    OTBPath

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation
    OTBInterpolation

  DESCRIPTION
    "${DOCUMENTATION}"
)
