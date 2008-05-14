PROJECT(ImageMarkovExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(Markov_EXAMPLES ${CXX_TEST_PATH}/otbMarkovExamplesTests)

#ADD_EXECUTABLE(testRandom testRandom.cxx )
#TARGET_LINK_LIBRARIES(testRandom OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovRandomField1Example MarkovRandomField1Example.cxx )
TARGET_LINK_LIBRARIES(MarkovRandomField1Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovRandomField2Example MarkovRandomField2Example.cxx )
TARGET_LINK_LIBRARIES(MarkovRandomField2Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

# ADD_EXECUTABLE(MarkovRandomField3Example MarkovRandomField3Example.cxx )
# TARGET_LINK_LIBRARIES(MarkovRandomField3Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Markov)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbMarkovExamplesTests)

SET(TOL 0.0)
SET(EPSILON 0.00000001)

ADD_TEST(MarkovRandomField1ExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
	${BASELINE}/MarkovRandomField1.png
	${TEMP}/MarkovRandomField1.png
        MarkovRandomField1ExampleTest
        ${INPUTDATA}/QB_Suburb.png
	${TEMP}/MarkovRandomField1.png
	1.0
	20
	1.0
)

ADD_TEST(MarkovRandomField2ExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
	${BASELINE}/MarkovRandomField2.png
	${TEMP}/MarkovRandomField2.png
        MarkovRandomField1ExampleTest
        ${INPUTDATA}/QB_Suburb.png
	${TEMP}/MarkovRandomField2.png
	1.0
	5
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMarkovExamplesTests otbMarkovExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbMarkovExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBMarkov)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

