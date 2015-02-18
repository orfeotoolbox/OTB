set(DOCUMENTATION "This module contains classes related to the reading and the
writing of vector data (see also VectorDataBase and VectorDataManipulation
modules).")

otb_module(OTBVectorDataIO 
  DEPENDS
    OTBVectorDataBase
    OTBVectorDataManipulation
    OTBCommon
    OTBIOGDAL
    OTBIOKML
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
