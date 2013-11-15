if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# Common generic tests
set(OBJECTDETECTION_TESTS1 ${CXX_TEST_PATH}/otbObjectDetectionTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbObjectDetectionTests1 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ------------------  otb::LabeledSampleLocalizationGenerator  -------------------

add_test(odTuLabeledSampleLocalizationGeneratorNew ${OBJECTDETECTION_TESTS1}
  otbLabeledSampleLocalizationGeneratorNew
  )

add_test(odTvLabeledSampleLocalizationGenerator ${OBJECTDETECTION_TESTS1}
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  otbLabeledSampleLocalizationGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  )

add_test(odTuDescriptorsListSampleGeneratorNew ${OBJECTDETECTION_TESTS1}
  otbDescriptorsListSampleGeneratorNew
  )

add_test(odTvDescriptorsListSampleGeneratorNoStreaming ${OBJECTDETECTION_TESTS1}
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

add_test(odTvDescriptorsListSampleGeneratorStreaming ${OBJECTDETECTION_TESTS1}
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

add_test(odTvDescriptorsSVMModelCreation ${OBJECTDETECTION_TESTS1}
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

add_test(odTuObjectDetectionClassifierNew ${OBJECTDETECTION_TESTS1}
  otbObjectDetectionClassifierNew
  )

add_test(odTvObjectDetectionClassifier ${OBJECTDETECTION_TESTS1}
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

add_test(odTvObjectDetectionClassifierStreaming ${OBJECTDETECTION_TESTS1}
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

add_test(odTuStandardMetaImageFunctionBuilderNew ${OBJECTDETECTION_TESTS1}
  otbStandardMetaImageFunctionBuilderNew
  )

add_test(odTvStandardMetaImageFunctionBuilder ${OBJECTDETECTION_TESTS1}
  otbStandardMetaImageFunctionBuilder
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01.tif
  )

# A enrichir
set(ObjectDetection_SRCS1
otbObjectDetectionTests1.cxx
otbLabeledSampleLocalizationGenerator.cxx
otbDescriptorsListSampleGenerator.cxx
otbObjectDetectionClassifier.cxx
otbStandardMetaImageFunctionBuilder.cxx
)


OTB_ADD_EXECUTABLE(otbObjectDetectionTests1 "${ObjectDetection_SRCS1}" "OTBObjectDetection;OTBIO;OTBTesting;OTBLearning")


endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
