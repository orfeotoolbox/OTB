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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Fusion)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

otb_add_test(NAME fuTeBayesianFusionImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/ImageBayesianFusion.tif
    ${TEMP}/ImageBayesianFusion.tif
  Execute $<TARGET_FILE:BayesianFusionImageFilter>
    ${INPUTDATA}/multiSpect.tif
    ${INPUTDATA}/multiSpectInterp.tif
    ${INPUTDATA}/panchro.tif
    ${TEMP}/ImageBayesianFusion.tif
    ${TEMP}/ImageBayesianFusion1.png
    ${TEMP}/ImageBayesianFusion2.png
    ${TEMP}/ImageBayesianFusion3.png
    ${TEMP}/ImageBayesianFusion4.png
    0.9999
)

otb_add_test(NAME fuTePanSharpeningExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/PanSharpeningExampleTest.tif
    ${TEMP}/PanSharpeningExampleTest.tif
  Execute $<TARGET_FILE:PanSharpeningExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
    ${TEMP}/PanSharpeningExampleTest.tif
    ${TEMP}/PanSharpeningExampleTest1.png
    ${TEMP}/PanSharpeningExampleTest2.png
    ${TEMP}/PanSharpeningExampleTest3.png
)
