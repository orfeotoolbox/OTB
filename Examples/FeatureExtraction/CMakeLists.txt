PROJECT(FeatureExtractionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(AlignmentsExample AlignmentsExample.cxx )
TARGET_LINK_LIBRARIES(AlignmentsExample OTBFeatureExtraction OTBCommon OTBIO
ITKIO)

ADD_EXECUTABLE(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx )
TARGET_LINK_LIBRARIES(TouziEdgeDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(HarrisExample HarrisExample.cxx )
TARGET_LINK_LIBRARIES(HarrisExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(ComplexMomentsImageFunctionExample ComplexMomentsImageFunctionExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(ComplexMomentPathExample ComplexMomentPathExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentPathExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(HuMomentsImageFunctionExample HuMomentsImageFunctionExample.cxx )
TARGET_LINK_LIBRARIES(HuMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(FlusserMomentsImageFunctionExample FlusserMomentsImageFunctionExample.cxx )
TARGET_LINK_LIBRARIES(FlusserMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(RatioLineDetectorExample RatioLineDetectorExample.cxx )
TARGET_LINK_LIBRARIES(RatioLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(CorrelationLineDetectorExample CorrelationLineDetectorExample.cxx )
TARGET_LINK_LIBRARIES(CorrelationLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(AssymmetricFusionOfLineDetectorExample
AssymmetricFusionOfLineDetectorExample.cxx)
TARGET_LINK_LIBRARIES(AssymmetricFusionOfLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(LocalHoughExample LocalHoughExample.cxx )
TARGET_LINK_LIBRARIES(LocalHoughExample OTBIO OTBFeatureExtraction OTBCommon)

ADD_EXECUTABLE(ExtractSegmentsByStepsExample
ExtractSegmentsByStepsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractSegmentsByStepsExample OTBFeatureExtraction OTBCommon OTBIO)

IF(OTB_USE_MAPNIK)
ADD_EXECUTABLE(LineSegmentDetectorExample LineSegmentDetectorExample.cxx)
TARGET_LINK_LIBRARIES(LineSegmentDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(RightAngleDetectionExample RightAngleDetectionExample.cxx)
TARGET_LINK_LIBRARIES(RightAngleDetectionExample OTBFeatureExtraction OTBCommon OTBIO)
ENDIF(OTB_USE_MAPNIK)

ADD_EXECUTABLE(ExtractSegmentsExample
ExtractSegmentsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractSegmentsExample OTBFeatureExtraction OTBCommon OTBIO)

ADD_EXECUTABLE(ExtractRoadByStepsExample ExtractRoadByStepsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractRoadByStepsExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(ExtractRoadExample ExtractRoadExample.cxx)
TARGET_LINK_LIBRARIES(ExtractRoadExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(SeamCarvingExample SeamCarvingExample.cxx)
TARGET_LINK_LIBRARIES(SeamCarvingExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(SeamCarvingOtherExample SeamCarvingOtherExample.cxx)
TARGET_LINK_LIBRARIES(SeamCarvingOtherExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(ThresholdToPointSetExample ThresholdToPointSetExample.cxx)
TARGET_LINK_LIBRARIES(ThresholdToPointSetExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)


ADD_EXECUTABLE(SURFExample SURFExample.cxx)
TARGET_LINK_LIBRARIES(SURFExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)


ADD_EXECUTABLE(ParallelLineDetectionExample ParallelLineDetectionExample.cxx)
TARGET_LINK_LIBRARIES(ParallelLineDetectionExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(CloudDetectionExample CloudDetectionExample.cxx)
TARGET_LINK_LIBRARIES(CloudDetectionExample OTBIO OTBCommon OTBFeatureExtraction ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(TextureExample TextureExample.cxx)
TARGET_LINK_LIBRARIES(TextureExample OTBIO OTBCommon OTBFeatureExtraction ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(PanTexExample PanTexExample.cxx)
TARGET_LINK_LIBRARIES(PanTexExample OTBIO OTBCommon OTBFeatureExtraction ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(SFSExample SFSExample.cxx)
TARGET_LINK_LIBRARIES(SFSExample OTBIO OTBCommon OTBFeatureExtraction ITKCommon ITKBasicFilters)


ADD_EXECUTABLE(EdgeDensityExample EdgeDensityExample.cxx)
TARGET_LINK_LIBRARIES(EdgeDensityExample OTBIO OTBCommon OTBBasicFilters ITKCommon ITKBasicFilters)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/FeatureExtraction)
SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(INPUTDATA_OTB ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS1 ${CXX_TEST_PATH}/otbFeatureExtractionExamplesTests1)
SET(EXE_TESTS2 ${CXX_TEST_PATH}/otbFeatureExtractionExamplesTests2)

SET(TOL 0.0)


# ------- CorrelationLineDetectorExampleTest----------

ADD_TEST(feTeCorrelationLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstLineCorrelations.png
        ${TEMP}/amstLineCorrelations.png
        ${BASELINE}/amstLineCorrelationDirections.png
        ${TEMP}/amstLineCorrelationDirections.png
        CorrelationLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineCorrelations.png
        ${TEMP}/amstLineCorrelationDirections.png
    5 1
)

# ------- LocalHoughExampleTest----------

ADD_TEST(feTeLocalHoughExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/detected_local_hough.png
        ${TEMP}/detected_local_hough.png
        LocalHoughExampleTest
        ${INPUTDATA}/detected_lines.png
        ${TEMP}/detected_local_hough.png
  30 10 1 50
)

# ------- HarrisExampleTest----------

ADD_TEST(feTeHarrisExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROISpot5Harris.png
        ${TEMP}/ROISpot5Harris.png
        HarrisExampleTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/ROISpot5Harris.png
    1.5 2 0.1
)

# ------- TouziEdgeDetectorExampleTest----------

ADD_TEST(feTeTouziEdgeDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstTouziEdges.png
        ${TEMP}/amstTouziEdges.png
        ${BASELINE}/amstTouziDirections.png
        ${TEMP}/amstTouziDirections.png
        TouziEdgeDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstTouziEdges.png
        ${TEMP}/amstTouziDirections.png
    3
)

# ------- ExtractSegmentsByStepsExampleTest----------

ADD_TEST(feTeExtractSegmentsByStepsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstSegmentExtractionBySteps.png
        ${TEMP}/amstSegmentExtractionBySteps.png
        ExtractSegmentsByStepsExampleTest
        ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
        ${TEMP}/amstSegmentExtractionBySteps.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- AssymmetricFusionOfLineDetectorExampleTest----------

ADD_TEST(feTeAssymmetricFusionOfLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstLineFusion.png
        ${TEMP}/amstLineFusion.png
        AssymmetricFusionOfLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineFusion.png
    5 1
)

# ------- ExtractSegmentsExampleTest----------

ADD_TEST(feTeExtractSegmentsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstSegmentExtraction.png
        ${TEMP}/amstSegmentExtraction.png
        ExtractSegmentsExampleTest
        ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
        ${TEMP}/amstSegmentExtraction.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- RatioLineDetectorExampleTest----------

ADD_TEST(feTeRatioLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstLineRatios.png
        ${TEMP}/amstLineRatios.png
        ${BASELINE}/amstLineRatioDirections.png
        ${TEMP}/amstLineRatioDirections.png
        RatioLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineRatios.png
        ${TEMP}/amstLineRatioDirections.png
    5 1
)

# ------- AlignmentsExampleTest----------

ADD_TEST(feTeAlignmentsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/QB_SuburbAlign.png
        ${TEMP}/QB_SuburbAlign.png
        AlignmentsExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/QB_SuburbAlign.png
        5
)

# ------- RoadExtractionExamplesTest----------


ADD_TEST(feTeExtractRoadByStepsExampleTest ${EXE_TESTS2}
        --compare-n-images ${TOL} 2
        ${BASELINE}/qb_ExtractRoad_pretty.png
        ${TEMP}/qb_ExtractRoad_pretty.png
        ${BASELINE}/ExtractRoadByStepsExampleOutput.png
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
        ExtractRoadByStepsExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
        ${TEMP}/qb_ExtractRoad_pretty.png
        337 557 432 859 0.00005 1.0
)


ADD_TEST(feTeExtractRoadExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/ExtractRoadExampleOutput.png
        ${TEMP}/ExtractRoadExampleOutput.png
        ExtractRoadExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadExampleOutput.png
        337 557 432 859 1.0 0.00005 1.0 0.39269 1.0 10.0 25.
)

# ------- SeamCarvingExamplesTest----------

ADD_TEST(feTeSeamCarvingExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/SeamCarvingExampleOutput.png
        ${TEMP}/SeamCarvingExampleOutput.png
        SeamCarvingExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/SeamCarvingExampleOutput.png
        50
)

ADD_TEST(feTeSeamCarvingOtherExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/SeamCarvingOtherExampleOutput.png
        ${TEMP}/SeamCarvingOtherExampleOutput.png
        SeamCarvingOtherExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/SeamCarvingOtherExampleOutput.png
        50
)

#ADD_TEST(feTeImageToSIFTKeyPointSetFilterTest ${EXE_TESTS}
#        --compare-image ${TOL}
#        ${BASELINE}/ImageToSIFTKeyPointSetFilterOutput.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        ImageToSIFTKeyPointSetFilterTest
#        ${INPUTDATA}/Scene.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        6 3 0.08 10
#)


# ------- CloudDetectionExampleTest----------

ADD_TEST(feTeCloudDetectionExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/CloudDetectionOutput.png
        ${TEMP}/PrettyCloudDetectionOutput.png
        CloudDetectionExampleTest
        ${INPUTDATA}/CloudsOnReunion.tif
        ${TEMP}/CloudDetectionOutput.tif
        ${TEMP}/PrettyCloudDetectionInput.png
        ${TEMP}/PrettyCloudDetectionOutput.png
        553 467 734 581 0.4 0.6 1.0
)

IF(OTB_USE_MAPNIK)

# ------- Line Segment Detector test----------

ADD_TEST(feTeLineSegmentDetectorExampleTest  ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/feTeLineSegmentDetectorExampleOutput.png
        ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
       LineSegmentDetectorExampleTest
        ${INPUTDATA}/Scene.png
        ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
)

# ------- Right Angle Detection test----------

ADD_TEST(feTeRightAngleDetectionExample  ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/feTeRightAngleDetectionExampleOutput.png
        ${TEMP}/feTeRightAngleDetectionExampleOutput.png
       RightAngleDetectionExample
        ${INPUTDATA}/Scene.png
        ${TEMP}/feTeRightAngleDetectionExampleOutput.png
        ${TEMP}/PrettyRightAngleInput.png
        ${TEMP}/PrettyRightAngleOutput.png
        0.1
        10.
)

ENDIF(OTB_USE_MAPNIK)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbFeatureExtractionExamplesTests1 otbFeatureExtractionExamplesTests1.cxx)
TARGET_LINK_LIBRARIES(otbFeatureExtractionExamplesTests1  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ADD_EXECUTABLE(otbFeatureExtractionExamplesTests2 otbFeatureExtractionExamplesTests2.cxx)
TARGET_LINK_LIBRARIES(otbFeatureExtractionExamplesTests2  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
