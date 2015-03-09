set(DOCUMENTATION "Change detection techniques try to detect and locate areas
which have changed between two or more observations of the same scene. These
changes can be of different types, with different origins and of different temporal
length. This module contains algorithms that can be used for different situations:
abrupt changes in an image pair, abrupt changes within an image series and a
known date, abrupt changes within an image series and an unknown date, progressive
changes within an image series.")

otb_module(OTBChangeDetection
  DEPENDS
    OTBCommon
    OTBStatistics
    OTBITK
    OTBProjection
    OTBImageManipulation

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
