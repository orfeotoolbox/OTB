set(DOCUMENTATION "This module contains a class that allow the fusion of
geometries in a layer (OGRLayer) along streaming lines, and a class that allow stream
segmentation and vectorization of the output label image.")

otb_module(OTBOGRProcessing
  DEPENDS
    OTBGdalAdapters
    OTBProjection
    OTBConversion
    OTBSmoothing
    OTBStreaming
    OTBCommon
    OTBLabelling
    OTBGDAL
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
