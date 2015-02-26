set(DOCUMENTATION "Map projections describe the link between geographic
coordinates and cartographic ones. So map projections allow representing a 2-dimensional
manifold of a 3-dimensional space (the Earth surface) in a 2-dimensional space
(a map which used to be a sheet of paper!). This module provides classes
generically related to this purpose. for instance, user can handle OGR data type, find RPC
solvers, re-project input vector data over input images, and so on.")

otb_module(OTBProjection
  DEPENDS
    OTBBoost
    OTBCommon
    OTBGDAL
    OTBGdalAdapters
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBMetadata
    OTBOSSIMAdapters
    OTBTransform
    OTBVectorDataBase
    OTBVectorDataManipulation

  TEST_DEPENDS
    OTBCommandLineParser
    OTBITKPendingPatches
    OTBImageIO
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
