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

set(OTBLabellingTests
otbLabellingTestDriver.cxx
otbLabelizeConfidenceConnectedImageFilter.cxx
otbLabelizeConnectedThresholdImageFilter.cxx
otbLabelizeNeighborhoodConnectedImageFilter.cxx
otbLabelToBoundaryImageFilter.cxx
)

add_executable(otbLabellingTestDriver ${OTBLabellingTests})
target_link_libraries(otbLabellingTestDriver ${OTBLabelling-Test_LIBRARIES})
otb_module_target_label(otbLabellingTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvLabelizeConfidenceConnectedImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  otbLabelizeConfidenceConnectedImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  127 127 2 3 1
  )

otb_add_test(NAME bfTvLabelizeConnectedThresholdImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  otbLabelizeConnectedThresholdImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  170 170
  80 80
  )

otb_add_test(NAME bfTvLabelizeNeighborhoodConnectedImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  otbLabelizeNeighborhoodConnectedImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  170 170
  80 80
  )

otb_add_test(NAME bfTvLabelToBoundaryImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelToBoundaryImageFilterOutput.tif
  ${TEMP}/bfTvLabelToBoundaryImageFilterOutput.tif
  otbLabelToBoundaryImageFilter
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/bfTvLabelToBoundaryImageFilterOutput.tif)

