PROJECT(FeatureExtractionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(AlignmentsExample AlignmentsExample.cxx )
TARGET_LINK_LIBRARIES(AlignmentsExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon
ITKIO)

ADD_EXECUTABLE(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx )
TARGET_LINK_LIBRARIES(TouziEdgeDetectorExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(HarrisExample HarrisExample.cxx )
TARGET_LINK_LIBRARIES(HarrisExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ComplexMomentImageExample ComplexMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentImageExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ComplexMomentPathExample ComplexMomentPathExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentPathExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(HuMomentImageExample HuMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(HuMomentImageExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(FlusserMomentImageExample FlusserMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(FlusserMomentImageExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(RatioLineDetectorExample RatioLineDetectorExample.cxx )
TARGET_LINK_LIBRARIES(RatioLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(CorrelationLineDetectorExample CorrelationLineDetectorExample.cxx )
TARGET_LINK_LIBRARIES(CorrelationLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(AssymmetricFusionOfLineDetectorExample
AssymmetricFusionOfLineDetectorExample.cxx)
TARGET_LINK_LIBRARIES(AssymmetricFusionOfLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(LocalHoughExample LocalHoughExample.cxx )
TARGET_LINK_LIBRARIES(LocalHoughExample ITKIO OTBIO OTBFeatureExtraction OTBCommon ITKCommon gdal)


ADD_EXECUTABLE(ExtractSegmentsByStepsExample
ExtractSegmentsByStepsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractSegmentsByStepsExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ExtractSegmentsExample
ExtractSegmentsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractSegmentsExample OTBFeatureExtraction OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(ExtractRoadByStepsExample ExtractRoadByStepsExample.cxx)
TARGET_LINK_LIBRARIES(ExtractRoadByStepsExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(ExtractRoadExample ExtractRoadExample.cxx)
TARGET_LINK_LIBRARIES(ExtractRoadExample OTBIO OTBCommon OTBFeatureExtraction
ITKCommon ITKBasicFilters)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/FeatureExtraction)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionExamplesTests)

SET(TOL 0.0)


# ------- CorrelationLineDetectorExampleTest----------

ADD_TEST(CorrelationLineDetectorExampleTest ${EXE_TESTS}
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

ADD_TEST(LocalHoughExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/detected_local_hough.png
	${TEMP}/detected_local_hough.png
	LocalHoughExampleTest
	${INPUTDATA}/detected_lines.png
	${TEMP}/detected_local_hough.png
  30 10 1 50
)

# ------- HarrisExampleTest----------

ADD_TEST(HarrisExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ROISpot5Harris.png
	${TEMP}/ROISpot5Harris.png
	HarrisExampleTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/ROISpot5Harris.png
    1.5 2 0.1
)

# ------- TouziEdgeDetectorExampleTest----------

ADD_TEST(TouziEdgeDetectorExampleTest ${EXE_TESTS}
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

ADD_TEST(ExtractSegmentsByStepsExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/amstSegmentExtractionBySteps.png
	${TEMP}/amstSegmentExtractionBySteps.png
	ExtractSegmentsByStepsExampleTest
	${INPUTDATA}/amst.png
	${TEMP}/amstSegmentExtractionBySteps.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- AssymmetricFusionOfLineDetectorExampleTest----------

ADD_TEST(AssymmetricFusionOfLineDetectorExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/amstLineFusion.png
	${TEMP}/amstLineFusion.png
	AssymmetricFusionOfLineDetectorExampleTest
	${INPUTDATA}/amst.png
	${TEMP}/amstLineFusion.png
    5 1 
)

# ------- ExtractSegmentsExampleTest----------

ADD_TEST(ExtractSegmentsExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/amstSegmentExtraction.png
	${TEMP}/amstSegmentExtraction.png
	ExtractSegmentsExampleTest
	${INPUTDATA}/amst.png
	${TEMP}/amstSegmentExtraction.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- RatioLineDetectorExampleTest----------

ADD_TEST(RatioLineDetectorExampleTest ${EXE_TESTS}
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

ADD_TEST(AlignmentsExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/QB_SuburbAlign.png
	${TEMP}/QB_SuburbAlign.png
	AlignmentsExampleTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/QB_SuburbAlign.png
)

# ------- RoadExtractionExamplesTest----------


ADD_TEST(ExtractRoadByStepsExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ExtractRoadByStepsExampleOutput.png
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
        ExtractRoadByStepsExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
	337 557 432 859 0.00005 1.0
)


ADD_TEST(ExtractRoadExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ExtractRoadExampleOutput.png
        ${TEMP}/ExtractRoadExampleOutput.png
        ExtractRoadExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadExampleOutput.png
	337 557 432 859 1.0 0.00005 1.0 0.39269 1.0 10.0 25.
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionExamplesTests otbFeatureExtractionExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbFeatureExtractionExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
