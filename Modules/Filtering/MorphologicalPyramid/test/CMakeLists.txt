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

set(OTBMorphologicalPyramidTests
otbMorphologicalPyramidTestDriver.cxx
otbMorphologicalPyramidAnalysisFilter.cxx
otbMorphologicalPyramidSegmenter.cxx
otbMorphologicalPyramidSynthesisFilter.cxx
otbMorphologicalPyramidMRToMSConverter.cxx
otbMorphologicalPyramidResampler.cxx
otbMorphologicalPyramidSegmentationFilter.cxx
)

add_executable(otbMorphologicalPyramidTestDriver ${OTBMorphologicalPyramidTests})
target_link_libraries(otbMorphologicalPyramidTestDriver ${OTBMorphologicalPyramid-Test_LIBRARIES})
otb_module_target_label(otbMorphologicalPyramidTestDriver)

# Tests Declaration

otb_add_test(NAME msTvMorphoPyrAnalysisFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${NOTOL} 5
  ${BASELINE}/msPyrAn_mire_log_an_2.png
  ${TEMP}/msPyrAn_mire_log_an_2.png
  ${BASELINE}/msPyrAn_mire_log_sf_2.png
  ${TEMP}/msPyrAn_mire_log_sf_2.png
  ${BASELINE}/msPyrAn_mire_log_if_2.png
  ${TEMP}/msPyrAn_mire_log_if_2.png
  ${BASELINE}/msPyrAn_mire_log_sd_2.png
  ${TEMP}/msPyrAn_mire_log_sd_2.png
  ${BASELINE}/msPyrAn_mire_log_id_2.png
  ${TEMP}/msPyrAn_mire_log_id_2.png
  otbMorphologicalPyramidAnalysisFilter
  ${INPUTDATA}/mire_log.png
  ${TEMP}/msPyrAn_mire_log
  png
  2
  2.0)

otb_add_test(NAME msTvMorphoPyrSegmentBright COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
  ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
  otbMorphologicalPyramidSegmenter
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
  0
  0.9
  0.9
  10
  )
set_property(TEST msTvMorphoPyrSegmentBright PROPERTY DEPENDS msTvMorphoPyrMRToMSConverter)

otb_add_test(NAME msTvMorphoPyrSegmentDark COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_if.tif
  ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
  otbMorphologicalPyramidSegmenter
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
  1
  0.9
  0.9
  10
  )
set_property(TEST msTvMorphoPyrSegmentDark PROPERTY DEPENDS msTvMorphoPyrMRToMSConverter)

otb_add_test(NAME msTvMorphoPyrSynthesisFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
  otbMorphologicalPyramidSynthesisFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
  4
  2.0)

otb_add_test(NAME msTvMorphoPyrMRToMSConverter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${EPSILON_8} 3
  ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
  ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
  ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif
  otbMorphologicalPyramidMRToMSConverter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
  ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif
  4
  2.0)

otb_add_test(NAME msTvMorphoPyrResampler COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/msPyrResampler_IKO_LesHalles_256_2.tif
  ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.tif
  ${BASELINE}/msPyrResampler_IKO_LesHalles_full.tif
  ${TEMP}/msPyrResampler_IKO_LesHalles_full.tif
  otbMorphologicalPyramidResampler
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.tif
  ${TEMP}/msPyrResampler_IKO_LesHalles_full.tif
  256
  256
  )

otb_add_test(NAME msTvMorphoPyrSegmentationFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${NOTOL} 4
  ${BASELINE}/msPyrSegmentation_IKO_Halles_2.tif
  ${TEMP}/msPyrSegmentation_IKO_Halles_2.tif
  ${BASELINE}/msPyrSegmentation_IKO_Halles_4.tif
  ${TEMP}/msPyrSegmentation_IKO_Halles_4.tif
  ${BASELINE}/msPyrSegmentation_IKO_Halles_5.tif
  ${TEMP}/msPyrSegmentation_IKO_Halles_5.tif
  ${BASELINE}/msPyrSegmentation_IKO_Halles_7.tif
  ${TEMP}/msPyrSegmentation_IKO_Halles_7.tif
  otbMorphologicalPyramidSegmentationFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSegmentation_IKO_Halles_
  tif
  4
  2.0
  0.9
  0.9
  10
  )

