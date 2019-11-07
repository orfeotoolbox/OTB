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

set(OTBLabelMapTests
otbLabelMapTestDriver.cxx
otbLabelObjectMapVectorizer.cxx
otbLabelImageToLabelMapWithAdjacencyFilter.cxx
otbImageToLabelMapWithAttributesFilter.cxx
otbKMeansAttributesLabelMapFilter.cxx
otbLabelMapToSampleListFilter.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilter.cxx
otbMinMaxAttributesLabelMapFilter.cxx
otbNormalizeAttributesLabelMapFilter.cxx
otbBandsStatisticsAttributesLabelMapFilter.cxx
)

add_executable(otbLabelMapTestDriver ${OTBLabelMapTests})
target_link_libraries(otbLabelMapTestDriver ${OTBLabelMap-Test_LIBRARIES})
otb_module_target_label(otbLabelMapTestDriver)

# Tests Declaration

otb_add_test(NAME obTvLabelObjectMapVectorizer COMMAND otbLabelMapTestDriver
  otbLabelObjectMapVectorizer
  ${INPUTDATA}/labelImage_UnsignedChar.tif
  otbLabelObjectMapVectorizerOutput.gml)

otb_add_test(NAME obTvLabelImageToLabelMapWithAdjacencyFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  otbLabelImageToLabelMapWithAdjacencyFilter
  ${INPUTDATA}/simpleLabelImage.tif
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  )

otb_add_test(NAME obTvImageToLabelMapWithAttributesFilter COMMAND otbLabelMapTestDriver
  otbImageToLabelMapWithAttributesFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif)
otb_add_test(NAME obTvKMeansAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  otbKMeansAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvKMeansAttributesLabelMapFilter.txt)
otb_add_test(NAME obTvLabelMapToSampleListFilter COMMAND otbLabelMapTestDriver
  otbLabelMapToSampleListFilter
  ${OTB_DATA_ROOT}/Input/labelImage_UnsignedChar.tif
  SHAPE::Flusser01 SHAPE::Flusser02 SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05 SHAPE::Flusser06 SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10 SHAPE::Flusser11)
otb_add_test(NAME obTvLabelMapWithClassLabelToLabeledSampleListFilter COMMAND otbLabelMapTestDriver
  otbLabelMapWithClassLabelToLabeledSampleListFilter
  ${OTB_DATA_ROOT}/Input/labelImage_UnsignedChar.tif
  SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)
otb_add_test(NAME obTvMinMaxAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvMinMaxAttributesLabelMapFilter.txt
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt
  otbMinMaxAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt)
otb_add_test(NAME obTvNormalizeAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvNormalizeAttributesLabelMapFilter.txt
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt
  otbNormalizeAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt)
otb_add_test(NAME obTvBandsStatisticsAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  otbBandsStatisticsAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvBandsStatisticsAttributesLabelMapFilter.txt)
