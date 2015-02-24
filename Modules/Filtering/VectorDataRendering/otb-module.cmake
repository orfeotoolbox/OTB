set(DOCUMENTATION "This module contains classes related to the rasterization of
OpenStreetMap vector data onto images")

otb_module(OTBVectorDataRendering
  DEPENDS
    OTBVectorDataManipulation
    OTBMapnik
    OTBITK
    OTBGDAL
    OTBCommon

  TEST_DEPENDS
    OTBVectorDataBase
    OTBTestKernel
    OTBImageIO
    OTBProjection
    OTBVectorDataIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
