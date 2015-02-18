set(DOCUMENTATION "This module contains base classes for representing vector
data (see also VectorDataIO and VectorDataManipulation modules).")

otb_module(OTBVectorDataBase 
  DEPENDS
    OTBCommon
    OTBOSSIMAdapters
    OTBGDAL
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
