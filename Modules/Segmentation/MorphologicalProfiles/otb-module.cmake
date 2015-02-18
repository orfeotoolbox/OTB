set(DOCUMENTATION "This module contains mono and multi-scale convex/concave
image classification based on geodesic morphology characteristics.")

otb_module(OTBMorphologicalProfiles 
  DEPENDS
    OTBImageBase
    OTBObjectList
    OTBCommon
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
