otb_module_test()

set(OTBObjectDetectionTests
otbObjectDetectionTestDriver.cxx
otbLabeledSampleLocalizationGenerator.cxx
otbStandardMetaImageFunctionBuilder.cxx
otbDescriptorsListSampleGenerator.cxx
)

add_executable(otbObjectDetectionTestDriver ${OTBObjectDetectionTests})
target_link_libraries(otbObjectDetectionTestDriver ${OTBObjectDetection-Test_LIBRARIES})
otb_module_target_label(otbObjectDetectionTestDriver)

# Tests Declaration

otb_add_test(NAME odTuLabeledSampleLocalizationGeneratorNew COMMAND otbObjectDetectionTestDriver
  otbLabeledSampleLocalizationGeneratorNew
  )

otb_add_test(NAME odTvLabeledSampleLocalizationGenerator COMMAND otbObjectDetectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  otbLabeledSampleLocalizationGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  )

otb_add_test(NAME odTuStandardMetaImageFunctionBuilderNew COMMAND otbObjectDetectionTestDriver
  otbStandardMetaImageFunctionBuilderNew
  )

otb_add_test(NAME odTvStandardMetaImageFunctionBuilder COMMAND otbObjectDetectionTestDriver
  otbStandardMetaImageFunctionBuilder
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01.tif
  )

otb_add_test(NAME odTvDescriptorsListSampleGeneratorStreaming COMMAND otbObjectDetectionTestDriver
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

#otb_add_test(NAME odTvDescriptorsSVMModelCreation COMMAND otbObjectDetectionTestDriver
  #--compare-ascii ${NOTOL}
  #${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  #${TEMP}/TvDescriptorsSVMModelCreation.svm
  #otbDescriptorsSVMModelCreation
  #${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_amplitude.tif
  #${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  #${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  #${TEMP}/TvDescriptorsSVMModelCreation.svm
  #0 # streaming
  #5 # neighborhood radius
  #)

otb_add_test(NAME odTvDescriptorsListSampleGeneratorNoStreaming COMMAND otbObjectDetectionTestDriver
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

otb_add_test(NAME odTuDescriptorsListSampleGeneratorNew COMMAND otbObjectDetectionTestDriver
  otbDescriptorsListSampleGeneratorNew
  )

