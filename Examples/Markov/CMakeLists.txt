PROJECT(ImageMarkovExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(Markov_EXAMPLES ${CXX_TEST_PATH}/otbMarkovExamplesTests)


ADD_EXECUTABLE(MarkovClassification1Example MarkovClassification1Example.cxx )
TARGET_LINK_LIBRARIES(MarkovClassification1Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovClassification2Example MarkovClassification2Example.cxx )
TARGET_LINK_LIBRARIES(MarkovClassification2Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Markov)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbMarkovExamplesTests)

SET(TOL 0.0)
SET(EPSILON 0.00000001)

ADD_TEST(MarkovClassification1ExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
	${BASELINE}/MarkovClassification1.png
	${TEMP}/MarkovClassification1.png
        MarkovClassification1ExampleTest
        ${INPUTDATA}/QB_Suburb.png
	${INPUTDATA}/RandomImage.png
	${TEMP}/MarkovClassification1.png
	1.0
	30
	1.0
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMarkovExamplesTests otbMarkovExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbMarkovExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBMarkov)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

