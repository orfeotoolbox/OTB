PROJECT(TutorialsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(TUTORIALS_EXAMPLES ${CXX_TEST_PATH}/otbTutorialsExamplesTests)

ADD_EXECUTABLE(HelloWorldOTB HelloWorldOTB.cxx )
TARGET_LINK_LIBRARIES(HelloWorldOTB OTBCommon OTBIO)

ADD_EXECUTABLE(Pipeline Pipeline.cxx )
TARGET_LINK_LIBRARIES(Pipeline OTBCommon OTBIO)

ADD_EXECUTABLE(FilteringPipeline FilteringPipeline.cxx )
TARGET_LINK_LIBRARIES(FilteringPipeline OTBCommon OTBIO)

ADD_EXECUTABLE(ScalingPipeline ScalingPipeline.cxx )
TARGET_LINK_LIBRARIES(ScalingPipeline OTBCommon OTBIO)

ADD_EXECUTABLE(SmarterFilteringPipeline SmarterFilteringPipeline.cxx )
TARGET_LINK_LIBRARIES(SmarterFilteringPipeline OTBCommon OTBIO)

ADD_EXECUTABLE(SimpleViewer SimpleViewer.cxx )
TARGET_LINK_LIBRARIES(SimpleViewer OTBCommon OTBIO OTBGui OTBVisu)

ADD_EXECUTABLE(OrthoFusion OrthoFusion.cxx )
TARGET_LINK_LIBRARIES(OrthoFusion OTBFusion OTBProjections OTBCommon OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Tutorials)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Examples/Tutorials/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbTutorialsExamplesTests)

SET(TOL 0.0)


ADD_TEST( TutorialsPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsPipelineOutput.png
                                        ${TEMP}/TutorialsPipelineOutput.png
        TutorialsPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsPipelineOutput.png
        )
ADD_TEST( TutorialsFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsFilteringPipelineOutput.png
        TutorialsFilteringPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsFilteringPipelineOutput.png
        )
ADD_TEST( TutorialsScalingPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsScalingPipelineOutput.png
                                        ${TEMP}/TutorialsScalingPipelineOutput.png
        TutorialsScalingPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsScalingPipelineOutput.png
        )

ADD_TEST( TutorialsSmarterFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsSmarterFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        TutorialsSmarterFilteringPipelineTest
        -in ${INPUTDATA}/QB_Suburb.png
        -out ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        -d 1.5 
        -i 2 
        -a 0.1
        )





INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbTutorialsExamplesTests otbTutorialsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbTutorialsExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
