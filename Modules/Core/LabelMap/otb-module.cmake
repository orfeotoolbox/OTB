set(DOCUMENTATION "This module contains classes that allow label images
manipulation based on object attributes. They are useful for Object-Based Image Analysis
(OBIA) (analyzing images at the object level instead of working at the pixel
level).")

otb_module(OTBLabelMap
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBMoments
    OTBVectorDataBase
    OTBVectorDataManipulation

  TEST_DEPENDS
    OTBImageIO
    OTBProjection
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
