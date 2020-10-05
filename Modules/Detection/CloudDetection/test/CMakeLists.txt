#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

set(OTBCloudDetectionTests
otbCloudDetectionTestDriver.cxx
otbCloudEstimatorDefaultFilter.cxx
otbCloudDetectionFilter.cxx
otbCloudEstimatorFilter.cxx
)

add_executable(otbCloudDetectionTestDriver ${OTBCloudDetectionTests})
target_link_libraries(otbCloudDetectionTestDriver ${OTBCloudDetection-Test_LIBRARIES})
otb_module_target_label(otbCloudDetectionTestDriver)

# Tests Declaration

otb_add_test(NAME feTvCloudEstimatorDefaultFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvSpectralAngleOutput.tif
  ${TEMP}/feTvSpectralAngleOutput.tif
  otbCloudEstimatorDefaultFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvSpectralAngleOutput.tif
  500
  731
  500
  632
  )

otb_add_test(NAME feTvCloudDetectionFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvCloudDetectionOutput.tif
  ${TEMP}/feTvCloudDetectionOutput.tif
  otbCloudDetectionFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvCloudDetectionOutput.tif
  500
  731
  500
  632
  0.25   # variance
  0.95   # minthreshold
  1.0    # maxthreshold
  )

otb_add_test(NAME feTvCloudEstimatorFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvCloudEstimatorOutput.tif
  ${TEMP}/feTvCloudEstimatorOutput.tif
  otbCloudEstimatorFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvCloudEstimatorOutput.tif
  500
  731
  500
  632
  0.25    # variance
  )

