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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Classification)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME clTeNeighborhoodMajorityVotingImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${OTB_DATA_ROOT}/Baseline/OTB/Images/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
    ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
  Execute $<TARGET_FILE:ClassificationMapRegularizationExample>
    ${OTB_DATA_ROOT}/Input/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
    ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
    false #KeepOriginalLabelBool
    2 #xRadius
    5 #yRadius
    10 #LabelForNoDataPixels
    7 #LabelForUndecidedPixels
)
