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

set(OTBMeanShiftTests
otbMeanShiftTestDriver.cxx
otbMeanShiftConnectedComponentSegmentationFilterTest.cxx
otbMeanShiftSegmentationFilter.cxx
)

add_executable(otbMeanShiftTestDriver ${OTBMeanShiftTests})
target_link_libraries(otbMeanShiftTestDriver ${OTBMeanShift-Test_LIBRARIES})
otb_module_target_label(otbMeanShiftTestDriver)

# Tests Declaration

otb_add_test(NAME obTuMeanShiftConnectedComponentSegmentationFilter COMMAND otbMeanShiftTestDriver
  otbMeanShiftConnectedComponentSegmentationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftConnectedComponentSegmentationImage.png
  9
  9
  0.01
  "distance<0.2"
  50)

otb_add_test(NAME obTuMeanShiftSegmentationFilterPruning COMMAND otbMeanShiftTestDriver
  otbMeanShiftSegmentationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterLabelImagePruning.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImagePruning.tif
  4
  25
  100
  0.1
  20
  )

otb_add_test(NAME obTuMeanShiftSegmentationFilter COMMAND otbMeanShiftTestDriver
  otbMeanShiftSegmentationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterLabelImage.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImage.tif
  4
  25
  100
  0.1
  )

