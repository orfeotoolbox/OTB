IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


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
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  0 # streaming
  5 # neighborhood radius
  )

ADD_TEST(odTvDescriptorsListSampleGeneratorStreaming ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvDescriptorsListSampleGeneratorNoStreamingOutput.txt
  ${TEMP}/TvDescriptorsListSampleGeneratorStreamingOutput.txt
  otbDescriptorsListSampleGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_amplitude.tif
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvDescriptorsListSampleGeneratorStreamingOutput.txt
  50 # streaming
  5 # neighborhood radius
  )

ADD_TEST(odTvDescriptorsSVMModelCreation ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  ${TEMP}/TvDescriptorsSVMModelCreation.svm
  otbDescriptorsSVMModelCreation
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_amplitude.tif
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  ${TEMP}/TvDescriptorsSVMModelCreation.svm
  0 # streaming
  5 # neighborhood radius
  )

ADD_TEST(odTuObjectDetectionClassifierNew ${OBJECTDETECTION_TESTS1}
  otbObjectDetectionClassifierNew
  )

ADD_TEST(odTvObjectDetectionClassifier ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvObjectDetectionClassifierOutput.txt
  ${TEMP}/TvObjectDetectionClassifierOutput.txt
  otbObjectDetectionClassifier
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_extract_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  ${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  ${TEMP}/TvObjectDetectionClassifierOutput.txt
  0 # streaming
  5 # neighborhood radius
  )

ADD_TEST(odTvObjectDetectionClassifierStreaming ${OBJECTDETECTION_TESTS1}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvObjectDetectionClassifierOutput.txt
  ${TEMP}/TvObjectDetectionClassifierOutputStreaming.txt
  otbObjectDetectionClassifier
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_extract_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  ${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  ${TEMP}/TvObjectDetectionClassifierOutputStreaming.txt
  50 # streaming
  5 # neighborhood radius
  )

ADD_TEST(odTuStandardMetaImageFunctionBuilderNew ${OBJECTDETECTION_TESTS1}
  otbStandardMetaImageFunctionBuilderNew
  )

ADD_TEST(odTvStandardMetaImageFunctionBuilder ${OBJECTDETECTION_TESTS1}
  otbStandardMetaImageFunctionBuilder
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01.tif
  )

# A enrichir
SET(ObjectDetection_SRCS1
otbObjectDetectionTests1.cxx
otbLabeledSampleLocalizationGenerator.cxx
otbDescriptorsListSampleGenerator.cxx
otbObjectDetectionClassifier.cxx
otbStandardMetaImageFunctionBuilder.cxx
)


OTB_ADD_EXECUTABLE(otbObjectDetectionTests1 "${ObjectDetection_SRCS1}" "OTBObjectDetection;OTBIO;OTBTesting;OTBLearning")


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )