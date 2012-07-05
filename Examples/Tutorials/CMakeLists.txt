PROJECT(TutorialsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(HelloWorldOTB HelloWorldOTB.cxx )
TARGET_LINK_LIBRARIES(HelloWorldOTB OTBCommon OTBIO)

ADD_EXECUTABLE(Pipeline Pipeline.cxx )
TARGET_LINK_LIBRARIES(Pipeline OTBCommon OTBIO)

ADD_EXECUTABLE(FilteringPipeline FilteringPipeline.cxx )
TARGET_LINK_LIBRARIES(FilteringPipeline OTBCommon OTBIO)

ADD_EXECUTABLE(ScalingPipeline ScalingPipeline.cxx )
TARGET_LINK_LIBRARIES(ScalingPipeline OTBCommon OTBIO)

ADD_EXECUTABLE(Multispectral Multispectral.cxx )
TARGET_LINK_LIBRARIES(Multispectral OTBCommon OTBIO)

ADD_EXECUTABLE(SmarterFilteringPipeline SmarterFilteringPipeline.cxx )
TARGET_LINK_LIBRARIES(SmarterFilteringPipeline OTBCommon OTBIO)

IF(OTB_USE_VISU_GUI)
  ADD_EXECUTABLE(SimpleViewer SimpleViewer.cxx )
  TARGET_LINK_LIBRARIES(SimpleViewer OTBCommon OTBIO OTBGui OTBVisualization ${OTB_VISU_GUI_LIBRARIES})

# The basic application tutorial makes use of the otbApplicationsCommon library which is built in OTB-Applications package.
# Therefore this tutorial will not compile until we move the OTBApplcationsCommon lib to the OTB. Until then,
# the following line will be commented out.
# SUBDIRS(BasicApplication)


ENDIF(OTB_USE_VISU_GUI)

ADD_EXECUTABLE(OrthoFusion OrthoFusion.cxx )
TARGET_LINK_LIBRARIES(OrthoFusion OTBFusion OTBProjections OTBCommon OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Tutorials)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbTutorialsExamplesTests)

SET(TOL 0.0)

ADD_TEST( trTeTutorialsHelloWorld ${EXE_TESTS}
        TutorialsHelloWorldOTBTest
        )

ADD_TEST( trTeTutorialsPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsPipelineOutput.png
                                        ${TEMP}/TutorialsPipelineOutput.png
        TutorialsPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsPipelineOutput.png
        )

ADD_TEST( trTeTutorialsFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsFilteringPipelineOutput.png
        TutorialsFilteringPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsFilteringPipelineOutput.png
        )

ADD_TEST( trTeTutorialsScalingPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsScalingPipelineOutput.png
                                        ${TEMP}/TutorialsScalingPipelineOutput.png
        TutorialsScalingPipelineTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/TutorialsScalingPipelineOutput.png
        )

ADD_TEST( trTeTutorialsMultispectralTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2      ${BASELINE}/MultispectralOutput1.tif
                                        ${TEMP}/MultispectralOutput1.tif
                                        ${BASELINE}/MultispectralOutput2.tif
                                        ${TEMP}/MultispectralOutput2.tif
        TutorialsMultispectralTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/MultispectralOutput1.tif
        ${TEMP}/MultispectralOutput2.tif
        )


ADD_TEST( trTeTutorialsSmarterFilteringPipelineTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/TutorialsSmarterFilteringPipelineOutput.png
                                        ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        TutorialsSmarterFilteringPipelineTest
        -in ${INPUTDATA}/QB_Suburb.png
        -out ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
        -d 1.5
        -i 2
        -a 0.1
        )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST( trTeTutorialsOrthoFusionTest ${EXE_TESTS}
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
ENDIF(OTB_DATA_USE_LARGEINPUT)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbTutorialsExamplesTests otbTutorialsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbTutorialsExamplesTests  ITKAlgorithms ITKStatistics ITKNumerics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
