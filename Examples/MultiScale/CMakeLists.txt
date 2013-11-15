project(MultiScaleExamples)
include_regular_expression("^.*$")

add_executable(MorphologicalPyramidAnalysisFilterExample MorphologicalPyramidAnalysisFilterExample.cxx )
target_link_libraries(MorphologicalPyramidAnalysisFilterExample OTBCommon OTBIO OTBMultiScale)

add_executable(MorphologicalPyramidSynthesisFilterExample MorphologicalPyramidSynthesisFilterExample.cxx )
target_link_libraries(MorphologicalPyramidSynthesisFilterExample OTBCommon OTBIO OTBMultiScale)

add_executable(MorphologicalPyramidSegmenterExample MorphologicalPyramidSegmenterExample.cxx )
target_link_libraries(MorphologicalPyramidSegmenterExample OTBCommon OTBIO OTBMultiScale )


add_executable(MorphologicalPyramidSegmentationExample MorphologicalPyramidSegmentationExample.cxx )
target_link_libraries(MorphologicalPyramidSegmentationExample OTBCommon OTBIO OTBMultiScale)



if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/MultiScale)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbMultiScaleExamplesTests)

set(TOL 0.0)


# ------- MorphologicalPyramidAnalysisFilterExampleTest ----------

add_test(msTeMorphologicalPyramidAnalysisFilterExampleTest ${EXE_TESTS}
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
        ${TEMP}/suburb2_an_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_an_2.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_an_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_an_4.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_sf_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sf_2.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_sf_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sf_4.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_if_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_if_2.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_if_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_if_4.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_id_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_id_2.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_id_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_id_4.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_sd_1.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sd_2.jpeg?&gdal:co:QUALITY=95
        ${TEMP}/suburb2_sd_3.jpeg?&gdal:co:QUALITY=95         ${TEMP}/suburb2_sd_4.jpeg?&gdal:co:QUALITY=95
        4 2
)

# ------- MorphologicalPyramidSynthesisFilterExampleTest ----------

add_test(msTeMorphologicalPyramidSynthesisFilterExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/suburb2_synthesis.jpeg
        ${TEMP}/suburb2_synthesis.jpeg
        MorphologicalPyramidSynthesisFilterExampleTest
        ${INPUTDATA}/suburb2.jpeg
        ${TEMP}/suburb2_synthesis.jpeg?&gdal:co:QUALITY=95
        4 2
)

# ------- MorphologicalPyramidSegmenterExampleTest ----------

add_test(msTeMorphologicalPyramidSegmenterExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/outputPyrSegmented.tif
        ${TEMP}/outputPyrSegmented.tif
        MorphologicalPyramidSegmenterExampleTest
        ${INPUTDATA}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/outputPyrSegmented.tif
        0 0.9 0.9 10
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbMultiScaleExamplesTests otbMultiScaleExamplesTests.cxx)
target_link_libraries(otbMultiScaleExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale  OTBTesting)

endif()
