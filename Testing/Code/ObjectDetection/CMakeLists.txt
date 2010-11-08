IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)

# Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#Tolerance
SET(NOTOL 0.0)

# Common generic tests
SET(OBJECTDETECTION_TESTS1 ${CXX_TEST_PATH}/otbObjectDetectionTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbObjectDetectionTests1 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ------------------  otb::LabeledSampleLocalizationGenerator  -------------------

ADD_TEST(odTuLabeledSampleLocalizationGeneratorNew ${OBJECTDETECTION_TESTS1}
  otbLabeledSampleLocalizationGeneratorNew
  )

ADD_TEST(odTvLabeledSampleLocalizationGenerator ${OBJECTDETECTION_TESTS1}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  otbLabeledSampleLocalizationGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  )

ADD_TEST(odTuDescriptorsListSampleGeneratorNew ${OBJECTDETECTION_TESTS1}
  otbDescriptorsListSampleGeneratorNew
  )

ADD_TEST(odTvDescriptorsListSampleGeneratorNoStreaming ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  ${TEMP}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  otbDescriptorsListSampleGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${TEMP}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  0
  )

ADD_TEST(odTvDescriptorsListSampleGeneratorStreaming ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  ${TEMP}/TvDescriptorsListSampleGeneratorStreamingOutput.txt
  otbDescriptorsListSampleGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${TEMP}/TvDescriptorsListSampleGeneratorStreamingOutput.txt
  50
  )

# A enrichir
SET(ObjectDetection_SRCS1
otbObjectDetectionTests1.cxx
otbLabeledSampleLocalizationGenerator.cxx
otbDescriptorsListSampleGenerator.cxx
)


OTB_ADD_EXECUTABLE(otbObjectDetectionTests1 "${ObjectDetection_SRCS1}" "OTBObjectDetection;OTBIO;OTBTesting")


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )