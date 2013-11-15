project(ImageMarkovExamples)
include_regular_expression("^.*$")

set(Markov_EXAMPLES ${CXX_TEST_PATH}/otbMarkovExamplesTests)

add_executable(MarkovClassification1Example MarkovClassification1Example.cxx )
target_link_libraries(MarkovClassification1Example OTBMarkov OTBCommon OTBIO )

add_executable(MarkovClassification2Example MarkovClassification2Example.cxx )
target_link_libraries(MarkovClassification2Example OTBMarkov OTBCommon OTBIO )

add_executable(MarkovClassification3Example MarkovClassification3Example.cxx )
target_link_libraries(MarkovClassification3Example OTBMarkov OTBCommon OTBIO )

add_executable(MarkovRestaurationExample MarkovRestaurationExample.cxx )
target_link_libraries(MarkovRestaurationExample OTBMarkov OTBCommon OTBIO )

add_executable(MarkovRegularizationExample MarkovRegularizationExample.cxx )
target_link_libraries(MarkovRegularizationExample OTBMarkov OTBCommon OTBIO )

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Markov)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbMarkovExamplesTests)

set(TOL 0.0)
set(EPSILON 0.00000001)

add_test(maTeMarkovClassification1ExampleTest ${EXE_TESTS}
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

add_test(maTeMarkovClassification2ExampleTest ${EXE_TESTS}
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

add_test(maTeMarkovClassification3ExampleTest ${EXE_TESTS}
        --compare-image ${EPSILON}
                    ${BASELINE}/MarkovClassification3.png
                    ${TEMP}/MarkovClassification3.png
    MarkovClassification3ExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MarkovClassification3.png
        ${TEMP}/MarkovClassification3_1.png
        1.0 20 1.0 1
)

add_test(maTeMarkovRestaurationExampleTest ${EXE_TESTS}
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


add_test(maTeMarkovRegularizationExampleTest ${EXE_TESTS}
        --compare-image ${EPSILON}
                      ${BASELINE}/MarkovRegularization-scaled.png
                      ${TEMP}/MarkovRegularization-scaled.png
   MarkovRegularizationExampleTest
       ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
       ${TEMP}/MarkovRegularization.png
       ${TEMP}/MarkovRegularization-scaled.png
       0.2
       20
       0.0
       1 # true to use always the same rand() sample, false otherwis
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbMarkovExamplesTests otbMarkovExamplesTests.cxx)
target_link_libraries(otbMarkovExamplesTests   ${ITK_LIBRARIES}  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBMarkov OTBTesting)


endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

