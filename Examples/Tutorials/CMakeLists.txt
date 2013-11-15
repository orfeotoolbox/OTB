project(TutorialsExamples)
include_regular_expression("^.*$")

add_executable(HelloWorldOTB HelloWorldOTB.cxx )
target_link_libraries(HelloWorldOTB OTBCommon OTBIO)

add_executable(Pipeline Pipeline.cxx )
target_link_libraries(Pipeline OTBCommon OTBIO)

add_executable(FilteringPipeline FilteringPipeline.cxx )
target_link_libraries(FilteringPipeline OTBCommon OTBIO)

add_executable(ScalingPipeline ScalingPipeline.cxx )
target_link_libraries(ScalingPipeline OTBCommon OTBIO)

add_executable(Multispectral Multispectral.cxx )
target_link_libraries(Multispectral OTBCommon OTBIO)

add_executable(SmarterFilteringPipeline SmarterFilteringPipeline.cxx )
target_link_libraries(SmarterFilteringPipeline OTBCommon OTBIO)

if(OTB_USE_VISU_GUI)
  add_executable(SimpleViewer SimpleViewer.cxx )
  target_link_libraries(SimpleViewer OTBCommon OTBIO OTBGui OTBVisualization ${OTB_VISU_GUI_LIBRARIES})

# The basic application tutorial makes use of the otbApplicationsCommon library which is built in OTB-Applications package.
# Therefore this tutorial will not compile until we move the OTBApplcationsCommon lib to the OTB. Until then,
# the following line will be commented out.
# subdirs(BasicApplication)


endif()

add_executable(OrthoFusion OrthoFusion.cxx )
target_link_libraries(OrthoFusion OTBFusion OTBProjections OTBCommon OTBIO)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Tutorials)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbTutorialsExamplesTests)

set(TOL 0.0)

add_test( trTeTutorialsHelloWorld ${EXE_TESTS}
        TutorialsHelloWorldOTBTest
        )

add_test( trTeTutorialsPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsPipelineOutput.png
                                        ${TEMP}/TutorialsPipelineOutput.png
        TutorialsPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsPipelineOutput.png
        )

add_test( trTeTutorialsFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsFilteringPipelineOutput.png
        TutorialsFilteringPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsFilteringPipelineOutput.png
        )

add_test( trTeTutorialsScalingPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsScalingPipelineOutput.png
                                        ${TEMP}/TutorialsScalingPipelineOutput.png
        TutorialsScalingPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsScalingPipelineOutput.png
        )

add_test( trTeTutorialsMultispectralTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2      ${BASELINE}/MultispectralOutput1.tif
                                        ${TEMP}/MultispectralOutput1.tif
                                        ${BASELINE}/MultispectralOutput2.tif
                                        ${TEMP}/MultispectralOutput2.tif
        TutorialsMultispectralTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/MultispectralOutput1.tif
        ${TEMP}/MultispectralOutput2.tif
        )


add_test( trTeTutorialsSmarterFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsSmarterFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        TutorialsSmarterFilteringPipelineTest
        -in ${INPUTDATA}/QB_Suburb.png
        -out ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        -d 1.5
        -i 2
        -a 0.1
        )

if(OTB_DATA_USE_LARGEINPUT)
add_test( trTeTutorialsOrthoFusionTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsOrthoFusionOutput.tif
                                        ${TEMP}/TutorialsOrthoFusionOutput.tif
        TutorialsOrthoFusionTest
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/TutorialsOrthoFusionOutput.tif
        31
        N
        375000
        4828100
        500
        500
        0.6
        -0.6
        )
endif()

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbTutorialsExamplesTests otbTutorialsExamplesTests.cxx)
target_link_libraries(otbTutorialsExamplesTests   ${ITK_LIBRARIES}  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

endif()
