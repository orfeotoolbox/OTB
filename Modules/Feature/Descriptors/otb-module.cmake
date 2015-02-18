set(DOCUMENTATION "The output of a descriptor algorithm is usually a short
vector of numbers which is invariant to common image transformations. Descriptors are
then compared according to some criterion in order to be matched. This module
contains different descriptors such as Fourrier-Mellin, SIFT, SURF, HOG, and so on.")

otb_module(OTBDescriptors 
  DEPENDS
    OTBSiftFast
    OTBITKPendingPatches
    OTBImageBase
    OTBTransform
    OTBPointSet
    OTBCommon
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
