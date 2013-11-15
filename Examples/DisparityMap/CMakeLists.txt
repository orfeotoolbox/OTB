project(DisparityMapExamples)
include_regular_expression("^.*$")

add_executable(SimpleDisparityMapEstimationExample SimpleDisparityMapEstimationExample.cxx )
target_link_libraries(SimpleDisparityMapEstimationExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(NCCRegistrationFilterExample NCCRegistrationFilterExample.cxx )
target_link_libraries(NCCRegistrationFilterExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(FineRegistrationImageFilterExample FineRegistrationImageFilterExample.cxx )
target_link_libraries(FineRegistrationImageFilterExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(StereoReconstructionExample StereoReconstructionExample.cxx )
target_link_libraries(StereoReconstructionExample OTBFeatureExtraction OTBCommon OTBIO)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbDisparityMapExamplesTests)

set(TOL 0.0)

# ------- SimpleDisparityMapEstimationExampleTest ----------

add_test(dmTeSimpleDisparityMapEstimationExampleTest ${EXE_TESTS}
 --compare-n-images ${TOL} 2
 ${BASELINE}/deformationFieldOutput.png
 ${TEMP}/deformationFieldOutput.png
 ${BASELINE}/resampledMovingOutput.png
 ${TEMP}/resampledMovingOutput.png
 SimpleDisparityMapEstimationExampleTest
 ${INPUTDATA}/StereoFixed.png
 ${INPUTDATA}/StereoMoving.png
 ${TEMP}/deformationFieldOutput.png
 ${TEMP}/resampledMovingOutput.png
 100 10 15 10 10 250 0.9
)

add_test(dmTeStereoReconstructionExampleTest ${EXE_TESTS}
 --compare-image ${TOL}
 ${BASELINE}/elevationOutput.tif
 ${TEMP}/elevationOutput.tif
 StereoReconstructionExampleTest
 ${INPUTDATA}/sensor_stereo_left.tif
 ${INPUTDATA}/sensor_stereo_right.tif
 ${TEMP}/elevationOutput.tif
 ${TEMP}/elevationOutputPrintable.png
 140
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbDisparityMapExamplesTests otbDisparityMapExamplesTests.cxx)
target_link_libraries(otbDisparityMapExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
