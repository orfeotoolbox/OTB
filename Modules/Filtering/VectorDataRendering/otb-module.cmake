set(DOCUMENTATION "This module contains classes related to the rasterization of
OpenStreetMap vector data onto images")

otb_module(OTBVectorDataRendering 
  DEPENDS
    OTBMapnik
    OTBVectorDataBase
    OTBVectorDataManipulation
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
