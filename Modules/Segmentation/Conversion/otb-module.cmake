set(DOCUMENTATION "This module contains tools that allow switching between
different segment representations (labelImage, labelMap, OGR Layer, QGIS table).
Rasterization and vectorization are important features of this module.")

otb_module(OTBConversion
  DEPENDS
    OTBVectorDataBase
    OTBVectorDataManipulation
    OTBGdalAdapters
    OTBITK
    OTBVectorDataIO
    OTBIOGDAL
    OTBStreaming
    OTBLabelMap
    OTBImageBase
    OTBMetadata
    OTBBoost
    OTBGDAL
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBProjection
    OTBSmoothing

  DESCRIPTION
    "${DOCUMENTATION}"
)
