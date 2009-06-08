PROJECT(OBIAExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageToLabelToImage ImageToLabelToImage.cxx )
TARGET_LINK_LIBRARIES(ImageToLabelToImage OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbTutorialsExamplesTests)

SET(TOL 0.0)

ADD_TEST( trTeTutorialsPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsPipelineOutput.png
                                        ${TEMP}/TutorialsPipelineOutput.png
        TutorialsPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsPipelineOutput.png
        )

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbOBIAExamplesTests otbOBIAExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbOBIAExamplesTests  ITKAlgorithms ITKStatistics ITKNumerics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
