PROJECT(DisparityMapExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(SimpleDisparityMapEstimationExample SimpleDisparityMapEstimationExample.cxx )
TARGET_LINK_LIBRARIES(SimpleDisparityMapEstimationExample OTBFeatureExtraction OTBCommon OTBIO
ITKIO)

ADD_EXECUTABLE(NCCRegistrationFilterExample NCCRegistrationFilterExample.cxx )
TARGET_LINK_LIBRARIES(NCCRegistrationFilterExample OTBFeatureExtraction OTBCommon OTBIO
ITKIO)

ADD_EXECUTABLE(FineRegistrationImageFilterExample FineRegistrationImageFilterExample.cxx )
TARGET_LINK_LIBRARIES(FineRegistrationImageFilterExample OTBFeatureExtraction OTBCommon OTBIO
ITKIO)

ADD_EXECUTABLE(StereoReconstructionExample StereoReconstructionExample.cxx )
TARGET_LINK_LIBRARIES(StereoReconstructionExample OTBFeatureExtraction OTBCommon OTBIO
ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbDisparityMapExamplesTests)

SET(TOL 0.0)

# ------- SimpleDisparityMapEstimationExampleTest ----------

ADD_TEST(dmTeSimpleDisparityMapEstimationExampleTest ${EXE_TESTS}
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

ADD_TEST(dmTeStereoReconstructionExampleTest ${EXE_TESTS}
 --compare-image ${TOL}
 ${BASELINE}/elevationOutput.tif
 ${TEMP}/elevationOutput.tif
 StereoReconstructionExampleTest
 ${INPUTDATA}/sensor_stereo_left.tif
 ${INPUTDATA}/sensor_stereo_right.tif
 ${TEMP}/elevationOutput.tif
 140
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbDisparityMapExamplesTests otbDisparityMapExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbDisparityMapExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
