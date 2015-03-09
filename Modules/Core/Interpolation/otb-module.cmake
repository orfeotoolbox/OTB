set(DOCUMENTATION "This module contains additional interpolation functions
(compared to the ones already available in itk -see itk::InterpolateImageFunction-).")

otb_module(OTBInterpolation
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase

  TEST_DEPENDS
    OTBImageIO
    OTBImageManipulation
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
