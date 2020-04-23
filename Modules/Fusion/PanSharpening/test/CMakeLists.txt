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

set(OTBPanSharpeningTests
otbPanSharpeningTestDriver.cxx
otbSimpleRcsPanSharpeningFusionImageFilter.cxx
otbBayesianFusionFilter.cxx
otbLmvmPanSharpeningFusionImageFilter.cxx
)

add_executable(otbPanSharpeningTestDriver ${OTBPanSharpeningTests})
target_link_libraries(otbPanSharpeningTestDriver ${OTBPanSharpening-Test_LIBRARIES})
otb_module_target_label(otbPanSharpeningTestDriver)

# Tests Declaration

otb_add_test(NAME fuTvSimpleRcsPanSharpeningFusionImageFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
  ${TEMP}/fuTvRcsPanSharpeningFusion.tif
  otbSimpleRcsPanSharpeningFusionImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/fuTvRcsPanSharpeningFusion.tif
  )



otb_add_test(NAME fuTvBayesianFusionFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/fuTvBayesianFusion.tif
  ${TEMP}/fuTvBayesianFusion.tif
  otbBayesianFusionFilter
  ${INPUTDATA}/multiSpect.tif
  ${INPUTDATA}/multiSpectInterp.tif
  ${INPUTDATA}/panchro.tif
  ${TEMP}/fuTvBayesianFusion.tif
  )



otb_add_test(NAME fuTvLmvmPanSharpeningFusionImageFilter COMMAND otbPanSharpeningTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
  ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
  otbLmvmPanSharpeningFusionImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/fuTvLmvmPanSharpeningFusion.tif
  )

