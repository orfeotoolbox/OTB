PROJECT(MultiScaleExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(MorphologicalPyramidAnalysisFilterExample MorphologicalPyramidAnalysisFilterExample.cxx )
TARGET_LINK_LIBRARIES(MorphologicalPyramidAnalysisFilterExample OTBCommon OTBIO OTBMultiScale ITKCommon ITKIO)

ADD_EXECUTABLE(MorphologicalPyramidSynthesisFilterExample MorphologicalPyramidSynthesisFilterExample.cxx )
TARGET_LINK_LIBRARIES(MorphologicalPyramidSynthesisFilterExample OTBCommon OTBIO OTBMultiScale ITKCommon ITKIO)

ADD_EXECUTABLE(MorphologicalPyramidSegmenterExample MorphologicalPyramidSegmenterExample.cxx )
TARGET_LINK_LIBRARIES(MorphologicalPyramidSegmenterExample OTBCommon OTBIO OTBMultiScale ITKCommon ITKIO)


ADD_EXECUTABLE(MorphologicalPyramidSegmentationExample MorphologicalPyramidSegmentationExample.cxx )
TARGET_LINK_LIBRARIES(MorphologicalPyramidSegmentationExample OTBCommon OTBIO OTBMultiScale ITKCommon ITKIO)



IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/MultiScale)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbMultiScaleExamplesTests)

SET(TOL 0.0)


# ------- MorphologicalPyramidAnalysisFilterExampleTest ----------

ADD_TEST(MorphologicalPyramidAnalysisFilterExampleTest ${EXE_TESTS}
	--compare-n-images ${TOL} 20
	${BASELINE}/suburb2_an_1.jpeg ${TEMP}/suburb2_an_1.jpeg 
	${BASELINE}/suburb2_an_2.jpeg ${TEMP}/suburb2_an_2.jpeg
	${BASELINE}/suburb2_an_3.jpeg ${TEMP}/suburb2_an_3.jpeg
	${BASELINE}/suburb2_an_4.jpeg ${TEMP}/suburb2_an_4.jpeg
	${BASELINE}/suburb2_sf_1.jpeg ${TEMP}/suburb2_sf_1.jpeg
	${BASELINE}/suburb2_sf_2.jpeg ${TEMP}/suburb2_sf_2.jpeg
	${BASELINE}/suburb2_sf_3.jpeg ${TEMP}/suburb2_sf_3.jpeg
	${BASELINE}/suburb2_sf_4.jpeg ${TEMP}/suburb2_sf_4.jpeg
	${BASELINE}/suburb2_if_1.jpeg ${TEMP}/suburb2_if_1.jpeg 
	${BASELINE}/suburb2_if_2.jpeg ${TEMP}/suburb2_if_2.jpeg 
	${BASELINE}/suburb2_if_3.jpeg ${TEMP}/suburb2_if_3.jpeg 
	${BASELINE}/suburb2_if_4.jpeg ${TEMP}/suburb2_if_4.jpeg
	${BASELINE}/suburb2_id_1.jpeg ${TEMP}/suburb2_id_1.jpeg 
	${BASELINE}/suburb2_id_2.jpeg ${TEMP}/suburb2_id_2.jpeg 
	${BASELINE}/suburb2_id_3.jpeg ${TEMP}/suburb2_id_3.jpeg 
	${BASELINE}/suburb2_id_4.jpeg ${TEMP}/suburb2_id_4.jpeg
	${BASELINE}/suburb2_sd_1.jpeg ${TEMP}/suburb2_sd_1.jpeg 
	${BASELINE}/suburb2_sd_2.jpeg ${TEMP}/suburb2_sd_2.jpeg 
	${BASELINE}/suburb2_sd_3.jpeg ${TEMP}/suburb2_sd_3.jpeg 
	${BASELINE}/suburb2_sd_4.jpeg ${TEMP}/suburb2_sd_4.jpeg
	MorphologicalPyramidAnalysisFilterExampleTest
	${INPUTDATA}/suburb2.jpeg
	${TEMP}/suburb2_an_1.jpeg 	${TEMP}/suburb2_an_2.jpeg
	${TEMP}/suburb2_an_3.jpeg 	${TEMP}/suburb2_an_4.jpeg
	${TEMP}/suburb2_sf_1.jpeg 	${TEMP}/suburb2_sf_2.jpeg 
	${TEMP}/suburb2_sf_3.jpeg 	${TEMP}/suburb2_sf_4.jpeg
	${TEMP}/suburb2_if_1.jpeg 	${TEMP}/suburb2_if_2.jpeg 
	${TEMP}/suburb2_if_3.jpeg 	${TEMP}/suburb2_if_4.jpeg
	${TEMP}/suburb2_id_1.jpeg 	${TEMP}/suburb2_id_2.jpeg 
	${TEMP}/suburb2_id_3.jpeg 	${TEMP}/suburb2_id_4.jpeg
	${TEMP}/suburb2_sd_1.jpeg 	${TEMP}/suburb2_sd_2.jpeg 
	${TEMP}/suburb2_sd_3.jpeg 	${TEMP}/suburb2_sd_4.jpeg
	4 2 
)

# ------- MorphologicalPyramidSynthesisFilterExampleTest ----------

ADD_TEST(MorphologicalPyramidSynthesisFilterExampleTest ${EXE_TESTS}
	--compare-image ${TOL} 
	${BASELINE}/suburb2_synthesis.jpeg
	${TEMP}/suburb2_synthesis.jpeg 
	MorphologicalPyramidSynthesisFilterExampleTest
	${INPUTDATA}/suburb2.jpeg
	${TEMP}/suburb2_synthesis.jpeg
	4 2 
)

# ------- MorphologicalPyramidSegmenterExampleTest ----------

ADD_TEST(MorphologicalPyramidSegmenterExampleTest ${EXE_TESTS}
	--compare-image ${TOL} 
	${BASELINE}/outputPyrSegmented.tif
	${TEMP}/outputPyrSegmented.tif 
	MorphologicalPyramidSegmenterExampleTest
	${INPUTDATA}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
	${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/outputPyrSegmented.tif
	0 0.9 0.9 10
)


ADD_EXECUTABLE(otbMultiScaleExamplesTests otbMultiScaleExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbMultiScaleExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )
