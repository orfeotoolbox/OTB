set(DOCUMENTATION "This module contains mono and multi-scale convex/concave
image classification based on geodesic morphology characteristics.")

otb_module(OTBMorphologicalProfiles
  DEPENDS
    OTBImageBase
    OTBImageManipulation
    OTBObjectList
    OTBITK
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
