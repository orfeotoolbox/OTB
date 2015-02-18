set(DOCUMENTATION "This module contains tools that allow switching between
different segment representations (labelImage, labelMap, OGR Layer, QGIS table).
Rasterization and vectorization are important features of this module.")

otb_module(OTBConversion 
  DEPENDS
    OTBVectorDataBase
    OTBLabelMap
    OTBGdalAdapters
    OTBVectorDataManipulation
    OTBIOGDAL
    OTBStreaming
    OTBCommon
    OTBVectorDataIO
    OTBImageBase
    OTBBoost
    OTBGDAL
  TEST_DEPENDS
    OTBTestKernel
    OTBProjection
    OTBSmoothing
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
