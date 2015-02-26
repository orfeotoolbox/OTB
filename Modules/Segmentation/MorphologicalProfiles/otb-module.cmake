set(DOCUMENTATION "This module contains mono and multi-scale convex/concave
image classification based on geodesic morphology characteristics.")

otb_module(OTBMorphologicalProfiles
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBObjectList

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
