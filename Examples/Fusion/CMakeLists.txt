PROJECT(ImageFusionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(FUSION_EXAMPLES ${CXX_TEST_PATH}/otbFusionExamplesTests)


ADD_EXECUTABLE(BayesianFusionImageFilter BayesianFusionImageFilter.cxx )
TARGET_LINK_LIBRARIES(BayesianFusionImageFilter OTBFusion OTBCommon OTBIO )

ADD_EXECUTABLE(PanSharpeningExample PanSharpeningExample.cxx )
TARGET_LINK_LIBRARIES(PanSharpeningExample OTBFusion OTBCommon OTBIO )


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Fusion)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbFusionExamplesTests)

SET(TOL 0.0)
SET(EPSILON 0.00000001)

ADD_TEST(fuTeBayesianFusionImageFilterTest ${EXE_TESTS}
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

ADD_TEST(fuTePanSharpeningExampleTest ${EXE_TESTS}
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

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbFusionExamplesTests otbFusionExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbFusionExamplesTests   ${ITK_LIBRARIES}  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
