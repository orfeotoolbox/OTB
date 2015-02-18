set(DOCUMENTATION "This module contains classes related to the handling of
vector data (see also VectorDataBase and VectorDataManipulation modules).")

otb_module(OTBVectorDataManipulation 
  DEPENDS
    OTBVectorDataBase
    OTBImageManipulation
    OTBTransform
    OTBOSSIMAdapters
    OTBCommon
    OTBGDAL
    OTBPath
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
