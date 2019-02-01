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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/ChangeDetection)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- CorrelChDetTest----------

otb_add_test(NAME cdTeCorrelChDetTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/CorrChDet.tif
    ${TEMP}/CorrChDet.tif
  Execute $<TARGET_FILE:CorrelChDet>
    ${INPUTDATA}/ERSBefore.png
    ${INPUTDATA}/ERSAfter.png
    ${TEMP}/CorrChDet.tif
    15
)

# ------- DiffChDetTest----------

otb_add_test(NAME cdTeDiffChDetTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/DiffChDet.tif
    ${TEMP}/DiffChDet.tif
  Execute $<TARGET_FILE:DiffChDet>
    ${INPUTDATA}/SpotBefore.png
    ${INPUTDATA}/SpotAfter.png
    ${TEMP}/DiffChDet.tif
    3
)

# ------- RatioChDetTest----------

otb_add_test(NAME cdTeRatioChDetTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/RatioChDet.tif
    ${TEMP}/RatioChDet.tif
  Execute $<TARGET_FILE:RatioChDet>
    ${INPUTDATA}/GomaAvant.png
    ${INPUTDATA}/GomaApres.png
    ${TEMP}/RatioChDet.tif
    3
)

# ------- KullbackLeiblerDistanceChDetTest----------

otb_add_test(NAME cdTeKullbackLeiblerDistanceChDetTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/KullbackLeiblerDistanceChDetTest.png
    ${TEMP}/KullbackLeiblerDistanceChDetTest.png
  Execute $<TARGET_FILE:KullbackLeiblerDistanceChDet>
    ${INPUTDATA}/GomaAvant.png
    ${INPUTDATA}/GomaApres.png
    ${TEMP}/KullbackLeiblerDistanceChDetTest.png
    35
)

# ------- KullbackLeiblerProfileChDetTest----------

otb_add_test(NAME cdTeKullbackLeiblerProfileChDetTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/KullbackLeiblerProfileChDetTest.png
    ${TEMP}/KullbackLeiblerProfileChDetTest.png
  Execute $<TARGET_FILE:KullbackLeiblerProfileChDet>
    ${INPUTDATA}/GomaAvant.png
    ${INPUTDATA}/GomaApres.png
    ${TEMP}/KullbackLeiblerProfileChDetTest.png
    5 51 1 12 24
)
