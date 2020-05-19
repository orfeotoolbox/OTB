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

set(OTBContrastTests
otbContrastTestDriver.cxx
otbComputeHistoFilter.cxx
otbApplyGainFilter.cxx
otbComputeGainLutFilter.cxx
otbCLHistogramEqualizationFilter.cxx
otbHelperCLAHE.cxx
)

add_executable(otbContrastTestDriver ${OTBContrastTests})
target_link_libraries(otbContrastTestDriver ${OTBContrast-Test_LIBRARIES})
otb_module_target_label(otbContrastTestDriver)

otb_add_test(NAME bfTvComputeHistoFilter COMMAND otbContrastTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvComputeHistoFilter.tif
  ${TEMP}/bfTvComputeHistoFilter.tif
  otbComputeHistoFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvComputeHistoFilter.tif
  )

otb_add_test(NAME bfTvComputeGainLutFilter COMMAND otbContrastTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvComputeGainLutFilter.tif
  ${TEMP}/bfTvComputeGainLutFilter.tif
  otbComputeGainLutFilter
  ${INPUTDATA}/QB_Suburb.png
  ${INPUTDATA}/inputHisto_QB_Suburb.tif
  ${TEMP}/bfTvComputeGainLutFilter.tif
  )

otb_add_test(NAME bfTvApplyGainFilter COMMAND otbContrastTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvApplyGainFilter.tif
  ${TEMP}/bfTvApplyGainFilter.tif
  otbApplyGainFilter
  ${INPUTDATA}/QB_Suburb.png
  ${INPUTDATA}/inputLut_QB_Suburb.tif
  ${TEMP}/bfTvApplyGainFilter.tif
  )

otb_add_test(NAME bfTvCLHistoEqFilter COMMAND otbContrastTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvApplyGainFilter.tif
  ${TEMP}/bfTvCLHistoEqFilter.tif
  otbCLHistogramEqualizationFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvCLHistoEqFilter.tif
  )