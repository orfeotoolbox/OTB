project(ImageFusionExamples)
include_regular_expression("^.*$")

set(FUSION_EXAMPLES ${CXX_TEST_PATH}/otbFusionExamplesTests)


add_executable(BayesianFusionImageFilter BayesianFusionImageFilter.cxx )
target_link_libraries(BayesianFusionImageFilter OTBFusion OTBCommon OTBIO )

add_executable(PanSharpeningExample PanSharpeningExample.cxx )
target_link_libraries(PanSharpeningExample OTBFusion OTBCommon OTBIO )


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Fusion)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbFusionExamplesTests)

set(TOL 0.0)
set(EPSILON 0.00000001)

add_test(fuTeBayesianFusionImageFilterTest ${EXE_TESTS}
        --compare-image ${EPSILON}
        ${BASELINE}/ImageBayesianFusion.tif
        ${TEMP}/ImageBayesianFusion.tif
    BayesianFusionImageFilterTest
        ${INPUTDATA}/multiSpect.tif
        ${INPUTDATA}/multiSpectInterp.tif
        ${INPUTDATA}/panchro.tif
        ${TEMP}/ImageBayesianFusion.tif
        ${TEMP}/ImageBayesianFusion1.png
        ${TEMP}/ImageBayesianFusion2.png
        ${TEMP}/ImageBayesianFusion3.png
        ${TEMP}/ImageBayesianFusion4.png
        0.9999
)

add_test(fuTePanSharpeningExampleTest ${EXE_TESTS}
    --compare-image ${EPSILON}  ${BASELINE}/PanSharpeningExampleTest.tif
                           ${TEMP}/PanSharpeningExampleTest.tif
        PanSharpeningExampleTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        ${TEMP}/PanSharpeningExampleTest.tif
        ${TEMP}/PanSharpeningExampleTest1.png
        ${TEMP}/PanSharpeningExampleTest2.png
        ${TEMP}/PanSharpeningExampleTest3.png
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbFusionExamplesTests otbFusionExamplesTests.cxx)
target_link_libraries(otbFusionExamplesTests   ${ITK_LIBRARIES}  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

endif()
