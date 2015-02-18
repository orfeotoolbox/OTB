set(DOCUMENTATION "This module contains classes that allow label images
manipulation based on object attributes. They are useful for Object-Based Image Analysis
(OBIA) (analyzing images at the object level instead of working at the pixel
level).")

otb_module(OTBLabelMap 
  DEPENDS
    OTBVectorDataBase
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
