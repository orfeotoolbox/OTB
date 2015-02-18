set(DOCUMENTATION "Map projections describe the link between geographic
coordinates and cartographic ones. So map projections allow representing a 2-dimensional
manifold of a 3-dimensional space (the Earth surface) in a 2-dimensional space
(a map which used to be a sheet of paper!). This module provides classes
generically related to this purpose. for instance, user can handle OGR data type, find RPC
solvers, re-project input vector data over input images, and so on.")

otb_module(OTBProjection 
  DEPENDS
    OTBVectorDataBase
    OTBGdalAdapters
    OTBImageBase
    OTBTransform
    OTBVectorDataManipulation
    OTBOSSIMAdapters
    OTBImageManipulation
    OTBMetadata
    OTBBoost
    OTBGDAL
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
