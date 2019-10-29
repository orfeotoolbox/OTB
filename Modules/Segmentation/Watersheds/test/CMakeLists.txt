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

set(OTBWatershedsTests
otbWatershedsTestDriver.cxx
otbWatershedSegmentationFilter.cxx
)

add_executable(otbWatershedsTestDriver ${OTBWatershedsTests})
target_link_libraries(otbWatershedsTestDriver ${OTBWatersheds-Test_LIBRARIES})
otb_module_target_label(otbWatershedsTestDriver)

# Tests Declaration

otb_add_test(NAME obTuWatershedSegmentationFilter COMMAND otbWatershedsTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/obTuWatershedSegmentationFilterLabelImage.tif
  ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
  otbWatershedSegmentationFilter
  ${INPUTDATA}/ROI_QB_PAN_1.tif
  ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
  0.01
  0.2
  )

