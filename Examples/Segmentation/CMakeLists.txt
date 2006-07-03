#
# Examples on the use of segmentation algorithms
#

PROJECT(ImageSegmentation)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(ConnectedThresholdImageFilter OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(OtsuThresholdImageFilter OtsuThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(OtsuThresholdImageFilter OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(OtsuMultipleThresholdImageFilter OtsuMultipleThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(OtsuMultipleThresholdImageFilter
                      OTBCommon OTBIO ITKCommon ITKIO ITKStatistics)

ADD_EXECUTABLE(ConfidenceConnected ConfidenceConnected.cxx )
TARGET_LINK_LIBRARIES(ConfidenceConnected OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(NeighborhoodConnectedImageFilter NeighborhoodConnectedImageFilter.cxx )
TARGET_LINK_LIBRARIES(NeighborhoodConnectedImageFilter OTBCommon OTBIO
ITKNumerics ITKIO)

ADD_EXECUTABLE(IsolatedConnectedImageFilter IsolatedConnectedImageFilter.cxx )
TARGET_LINK_LIBRARIES(IsolatedConnectedImageFilter OTBCommon OTBIO ITKNumerics ITKIO)

#ADD_EXECUTABLE(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ConnectedThresholdImageFilter ITKNumerics ITKIO)

ADD_EXECUTABLE(FastMarchingImageFilter FastMarchingImageFilter.cxx )
TARGET_LINK_LIBRARIES(FastMarchingImageFilter OTBCommon OTBIO ITKNumerics ITKIO)

#ADD_EXECUTABLE(GeodesicActiveContourShapePriorLevelSetImageFilter GeodesicActiveContourShapePriorLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GeodesicActiveContourShapePriorLevelSetImageFilter ITKNumerics ITKIO ITKStatistics)

#ADD_EXECUTABLE(DeformableModel2 DeformableModel2.cxx )
#TARGET_LINK_LIBRARIES(DeformableModel2 ITKAlgorithms ITKNumerics ITKIO)

#ADD_EXECUTABLE(GibbsPriorImageFilter1 GibbsPriorImageFilter1.cxx )
#TARGET_LINK_LIBRARIES(GibbsPriorImageFilter1 ITKAlgorithms ITKNumerics ITKIO)

#ADD_EXECUTABLE(HoughTransform2DCirclesImageFilter HoughTransform2DCirclesImageFilter.cxx )
#TARGET_LINK_LIBRARIES(HoughTransform2DCirclesImageFilter ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(HoughTransform2DLinesImageFilter HoughTransform2DLinesImageFilter.cxx )
#TARGET_LINK_LIBRARIES(HoughTransform2DLinesImageFilter ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(VectorConfidenceConnected VectorConfidenceConnected.cxx )
#TARGET_LINK_LIBRARIES(VectorConfidenceConnected ITKNumerics ITKIO)

#ADD_EXECUTABLE(RelabelComponentImageFilter RelabelComponentImageFilter.cxx )
#TARGET_LINK_LIBRARIES(RelabelComponentImageFilter ITKNumerics ITKIO)


#ADD_EXECUTABLE(CannySegmentationLevelSetImageFilter CannySegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CannySegmentationLevelSetImageFilter ITKNumerics ITKIO)

#ADD_EXECUTABLE(GeodesicActiveContourImageFilter GeodesicActiveContourImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GeodesicActiveContourImageFilter ITKNumerics ITKIO)

#ADD_EXECUTABLE(ShapeDetectionLevelSetFilter ShapeDetectionLevelSetFilter.cxx )
#TARGET_LINK_LIBRARIES(ShapeDetectionLevelSetFilter ITKNumerics ITKIO)

#ADD_EXECUTABLE(SegmentationExamples2 SegmentationExamples2.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples2 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)

#ADD_EXECUTABLE(SegmentationExamples3 SegmentationExamples3.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples3 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)

#ADD_EXECUTABLE(CurvesLevelSetImageFilter CurvesLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvesLevelSetImageFilter ITKNumerics ITKIO)

ADD_EXECUTABLE(WatershedSegmentation WatershedSegmentation.cxx )
TARGET_LINK_LIBRARIES(WatershedSegmentation OTBCommon OTBIO
OTBSegmentation ITKAlgorithms ITKNumerics ITKIO)


#ADD_EXECUTABLE(SegmentationExamples SegmentationExamples.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon)



#ADD_EXECUTABLE(SegmentationExamples5 SegmentationExamples5.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples5 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)


#ADD_EXECUTABLE(SegmentationExamples8 SegmentationExamples8.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples8 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)

#ADD_EXECUTABLE(SegmentationExamples9 SegmentationExamples9.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples9 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)

#ADD_EXECUTABLE(CellularSegmentation1 CellularSegmentation1.cxx )
#TARGET_LINK_LIBRARIES(CellularSegmentation1 ITKAlgorithms ITKCommon ITKNumerics ITKIO)

#ADD_EXECUTABLE(DeformableModel1 DeformableModel1.cxx )
#TARGET_LINK_LIBRARIES(DeformableModel1 ITKAlgorithms ITKNumerics ITKIO)

#ADD_EXECUTABLE(LaplacianSegmentationLevelSetImageFilter LaplacianSegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LaplacianSegmentationLevelSetImageFilter ITKNumerics ITKIO)

#ADD_EXECUTABLE(ThresholdSegmentationLevelSetImageFilter ThresholdSegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ThresholdSegmentationLevelSetImageFilter ITKNumerics ITKIO)

#ADD_EXECUTABLE(SegmentationExamples4 SegmentationExamples4.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples4 ITKAlgorithms ITKIO ITKBasicFilters ITKCommonITKStatistics)

#ADD_EXECUTABLE(SegmentationExamples6 SegmentationExamples6.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples6 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)



#ADD_EXECUTABLE(SegmentationExamples7 SegmentationExamples7.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples7 ITKAlgorithms ITKIO
#ITKBasicFilters ITKCommon ITKStatistics)



