PROJECT(ImageFusionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(FUSION_EXAMPLES ${CXX_TEST_PATH}/otbFusionExamplesTests)


ADD_EXECUTABLE(BayesianFusionImageFilter BayesianFusionImageFilter.cxx )
TARGET_LINK_LIBRARIES(BayesianFusionImageFilter OTBFusion OTBCommon OTBIO ITKNumerics ITKIO)


IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Fusion)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Examples/Fusion/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbFusionExamplesTests)

SET(TOL 0.0)

ADD_TEST(BayesianFusionImageFilterTest ${EXE_TESTS}
	--compare-image ${TOL}
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



INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFusionExamplesTests otbFusionExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbFusionExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

