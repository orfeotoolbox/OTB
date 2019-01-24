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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Patented)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# -----EstimateAffineTransformationExampleTest---------
otb_add_test(NAME dmTeEstimateAffineTransformationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/EstimateAffineTransformationOutput.png
    ${TEMP}/AffineTransformationOutput.png
  Execute $<TARGET_FILE:EstimateAffineTransformationExample>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbR10X13Y17.png
    ${TEMP}/AffineTransformationOutput.png
    ${TEMP}/AffineTransformationTxtOutput.txt
    2 3 0 0 0.5 1
)

# -----SIFTDisparityMapEstimationTest---------
if(Module_OTBSiftFast)

  otb_add_test(NAME dmTeSIFTDisparityMapEstimationTest COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
      ${BASELINE}/SIFTdeformationFieldOutput.png
      ${TEMP}/SIFTdeformationFieldOutput.png
    Execute $<TARGET_FILE:SIFTDisparityMapEstimation>
      ${INPUTDATA}/ROISpot5.png
      ${INPUTDATA}/ROISpot5Warped.png
      ${TEMP}/SIFTdeformationFieldOutput.png
  )

  otb_add_test(NAME feTeSIFTFastExampleTest COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
     ${BASELINE}/ROISpot5SiftFast.png
     ${TEMP}/ROISpot5SiftFast.png
    Execute $<TARGET_FILE:SIFTFastExample>
     ${INPUTDATA}/ROISpot5.png
     ${TEMP}/ROISpot5SiftFast.png
     3
  )

endif()
