set(DOCUMENTATION "If the smoothing filter used in the pyramidal analysis is a
morphological filter, one cannot safely subsample the filtered image without loss
of information. However, by keeping the details possibly lost in the
down-sampling operation, such a decomposition can be used. The Morphological Pyramid is an
approach to such a decomposition. This module contains classes useful in meeting
that goal.")

otb_module(OTBMorphologicalPyramid
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBObjectList
    OTBPointSet

  TEST_DEPENDS
    OTBImageIO
    OTBMorphologicalProfiles
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
