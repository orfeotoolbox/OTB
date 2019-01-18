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

set(OTBMetricsTests
otbMetricsTestDriver.cxx
otbHooverInstanceFilterToAttributeImage.cxx
otbHooverMatrixFilter.cxx
)

add_executable(otbMetricsTestDriver ${OTBMetricsTests})
target_link_libraries(otbMetricsTestDriver ${OTBMetrics-Test_LIBRARIES})
otb_module_target_label(otbMetricsTestDriver)

# Tests Declaration

otb_add_test(NAME obTvHooverInstanceFilterToAttributeImage COMMAND otbMetricsTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
  ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
  otbHooverInstanceFilterToAttributeImage
  ${INPUTDATA}/maur_GT.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
  )

otb_add_test(NAME obTvHooverMatrixFilter COMMAND otbMetricsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
  ${TEMP}/obTvHooverMatrixFilter.txt
  otbHooverMatrixFilter
  ${INPUTDATA}/Seg1InputForRCC8Graph.tif
  ${INPUTDATA}/Seg2InputForRCC8Graph.tif
  ${TEMP}/obTvHooverMatrixFilter.txt
  )

