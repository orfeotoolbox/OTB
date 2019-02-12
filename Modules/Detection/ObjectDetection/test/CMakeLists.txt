#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBObjectDetectionTests
otbObjectDetectionTestDriver.cxx
otbLabeledSampleLocalizationGenerator.cxx
otbStandardMetaImageFunctionBuilder.cxx
otbDescriptorsListSampleGenerator.cxx
otbObjectDetectionClassifier.cxx
)

add_executable(otbObjectDetectionTestDriver ${OTBObjectDetectionTests})
target_link_libraries(otbObjectDetectionTestDriver ${OTBObjectDetection-Test_LIBRARIES})
otb_module_target_label(otbObjectDetectionTestDriver)

# Tests Declaration


otb_add_test(NAME odTvLabeledSampleLocalizationGenerator COMMAND otbObjectDetectionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/TvLabeledSampleLocalizationGeneratorOutput.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
  otbLabeledSampleLocalizationGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01LabeledPoints.shp
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/TvLabeledSampleLocalizationGeneratorOutput.shp
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



otb_add_test(NAME odTvObjectDetectionClassifierStreaming COMMAND otbObjectDetectionTestDriver
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

otb_add_test(NAME odTvObjectDetectionClassifier COMMAND otbObjectDetectionTestDriver
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
