PROJECT(BasicFiltersExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(LeeImageFilter LeeImageFilter.cxx )
TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(DEMToRainbowExample DEMToRainbowExample.cxx )
TARGET_LINK_LIBRARIES(DEMToRainbowExample OTBCommon OTBIO)

ADD_EXECUTABLE(ScalingFilterExample ScalingFilterExample.cxx )
TARGET_LINK_LIBRARIES(ScalingFilterExample OTBCommon OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbBasicFiltersExamplesTests)

SET(TOL 0.0)


# ------- LeeImageFilterTest----------

ADD_TEST(LeeImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/GomaSmallLeeFiltered.png
	${TEMP}/GomaSmallLeeFiltered.png
	LeeImageFilterTest
	${INPUTDATA}/GomaSmall.png
	${TEMP}/GomaSmallLeeFiltered.png
    3 1
)

# ------- DEMToRainbowExampleTest ----------

ADD_TEST(DEMToRainbowExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/DEMToRainbowImageGenerator.png
	${TEMP}/DEMToRainbowImageGenerator.png
	DEMToRainbowExampleTest
	${TEMP}/DEMToRainbowImageGenerator.png
    6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)


# ------- LeeImageFilterTest----------

ADD_TEST(ScalingFilterExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/QB_Toulouse_Ortho_PAN_rescaled.png
	${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
	${BASELINE}/QB_Toulouse_Ortho_PAN_casted.png
	${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
	ScalingFilterExampleTest
	${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
	${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
	${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
)

ADD_EXECUTABLE(otbBasicFiltersExamplesTests otbBasicFiltersExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbBasicFiltersExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
