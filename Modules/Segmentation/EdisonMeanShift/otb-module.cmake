set(DOCUMENTATION "This module implements the mean shift segmentation algorithm
(EDISON library).")

otb_module(OTBEdisonMeanShift 
  DEPENDS
    OTBVectorDataBase
    OTBImageBase
    OTBEdison
    OTBCommon
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
