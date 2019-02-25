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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Filtering)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

# ------- DanielssonDistanceMapImageFilterTest----------

otb_add_test(NAME fiTeDanielssonDistanceMapImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/DanielssonDistanceMapImageFilterOutput1.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
    ${BASELINE}/DanielssonDistanceMapImageFilterOutput2.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
  Execute $<TARGET_FILE:DanielssonDistanceMapImageFilter>
    ${INPUTDATA}/FivePoints.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
)

