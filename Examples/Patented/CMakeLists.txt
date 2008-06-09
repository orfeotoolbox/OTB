PROJECT(PatentedExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")


ADD_EXECUTABLE(FuzzyConnectednessImageFilter FuzzyConnectednessImageFilter.cxx )
TARGET_LINK_LIBRARIES(FuzzyConnectednessImageFilter OTBIO OTBCommon ITKNumerics ITKIO)

#ADD_EXECUTABLE(HybridSegmentationFuzzyVoronoi HybridSegmentationFuzzyVoronoi.cxx )
#TARGET_LINK_LIBRARIES(HybridSegmentationFuzzyVoronoi OTBIO OTBCommon ITKNumerics ITKIO)



IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Patented)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbPatentedExamplesTests)

SET(TOL 0.0)


# ------- HybridSegmentationFuzzyVoronoiTest----------

ADD_TEST(HybridSegmentationFuzzyVoronoiTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/HybridSegmentationFuzzyVoronoiOutput.png
	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
	HybridSegmentationFuzzyVoronoiTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
    111 38 75 20 0.5 3.0
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbPatentedExamplesTests otbPatentedExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbPatentedExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )
