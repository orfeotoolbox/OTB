set(DOCUMENTATION "This module contains tools related to edge / line
extraction. users can find classes that implement tools such as Sobel Detector, Touzi or
Line Ratio Detector (SAR images), Hough Transform, and so on.")

otb_module(OTBEdge
  DEPENDS
    OTBCommon
    OTBConversion
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBImageManipulation
    OTBObjectList
    OTBPath
    OTBProjection
    OTBSpatialObjects
    OTBStreaming
    OTBVectorDataBase

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageIO
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
