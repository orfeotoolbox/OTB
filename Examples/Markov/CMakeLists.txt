PROJECT(ImageMarkovExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(Markov_EXAMPLES ${CXX_TEST_PATH}/otbMarkovExamplesTests)

ADD_EXECUTABLE(MarkovClassification1Example MarkovClassification1Example.cxx )
TARGET_LINK_LIBRARIES(MarkovClassification1Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovClassification2Example MarkovClassification2Example.cxx )
TARGET_LINK_LIBRARIES(MarkovClassification2Example OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovRestaurationExample MarkovRestaurationExample.cxx )
TARGET_LINK_LIBRARIES(MarkovRestaurationExample OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(MarkovRegularizationExample MarkovRegularizationExample.cxx )
TARGET_LINK_LIBRARIES(MarkovRegularizationExample OTBMarkov OTBCommon OTBIO ITKNumerics ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Markov)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
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
        ${TEMP}/MarkovClassification1.png
        1.0
        20
        1.0
	1 #true to use always the same rand() sample, false otherwise
)

ADD_TEST(MarkovClassification2ExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
                    ${BASELINE}/MarkovClassification2.png
                    ${TEMP}/MarkovClassification2.png
    MarkovClassification2ExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MarkovClassification2.png
        1.0
	5
	1 # true to use always the same rand() sample, false otherwise
)

ADD_TEST(MarkovRestaurationExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
	                ${BASELINE}/MarkovRestauration.png
	                ${TEMP}/MarkovRestauration.png
    MarkovRestaurationExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MarkovRestauration.png
        10.0
        30.0
        1.
	1 # true to use always the same rand() sample, false otherwise
)


ADD_TEST(MarkovRegularizationExampleTest ${EXE_TESTS}
	--compare-image ${EPSILON}
                      ${BASELINE}/MarkovRegularization-scaled.png
                      ${TEMP}/MarkovRegularization-scaled.png
   MarkovRegularizationExampleTest
       ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
       ${TEMP}/MarkovRegularization.png
       ${TEMP}/MarkovRegularization-scaled.png
       0.2
       20
       1.0
       1 # true to use always the same rand() sample, false otherwis
)


ADD_EXECUTABLE(otbMarkovExamplesTests otbMarkovExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbMarkovExamplesTests  ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBMarkov)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

