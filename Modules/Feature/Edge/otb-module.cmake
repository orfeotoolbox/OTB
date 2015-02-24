set(DOCUMENTATION "This module contains tools related to edge / line
extraction. users can find classes that implement tools such as Sobel Detector, Touzi or
Line Ratio Detector (SAR images), Hough Transform, and so on.")

otb_module(OTBEdge
  DEPENDS
    OTBVectorDataBase
    OTBConversion
    OTBProjection
    OTBITKPendingPatches
    OTBImageBase
    OTBSpatialObjects
    OTBStreaming
    OTBImageManipulation
    OTBPath
    OTBCommon
    OTBObjectList
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBConvolution
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
