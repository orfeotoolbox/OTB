set(DOCUMENTATION "This module implements the mean shift segmentation algorithm
(EDISON library).")

otb_module(OTBEdisonMeanShift
  DEPENDS
    OTBVectorDataBase
    OTBEdison
    OTBITK
    OTBImageBase
    OTBCommon
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation

  DESCRIPTION
    "${DOCUMENTATION}"
)
