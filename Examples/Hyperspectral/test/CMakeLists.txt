#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Hyperspectral)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME siTvHyperspectralExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 3
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand3.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
  Execute $<TARGET_FILE:HyperspectralUnmixingExample>
    ${INPUTDATA}/AVIRIS/Indian_pines_corrected.tif
    ${TEMP}/siTvHyperspectralUnmixingExampleTest.tif
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
    16
)
