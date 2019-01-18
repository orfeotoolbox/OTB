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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Registration)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME reTeImageRegistration1ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration1Output.png
    ${TEMP}/ImageRegistration1Output.png
  Execute $<TARGET_FILE:ImageRegistration1>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_Suburb13x17y.png
    ${TEMP}/ImageRegistration1Output.png
    ${TEMP}/ImageRegistration1DifferenceAfter.png
    ${TEMP}/ImageRegistration1DifferenceBefore.png
)

otb_add_test(NAME reTeImageRegistration2ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration2Output.png
    ${TEMP}/ImageRegistration2Output.png
  Execute $<TARGET_FILE:ImageRegistration2>
    ${INPUTDATA}/RamsesROISmall.png
    ${INPUTDATA}/ADS40RoiSmall.png
    ${TEMP}/ImageRegistration2Output.png
    ${TEMP}/ImageRegistration2CheckerboardBefore.png
    ${TEMP}/ImageRegistration2CheckerboardAfter.png
)

otb_add_test(NAME reTeImageRegistration5Cas1ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration5Output1.png
    ${TEMP}/ImageRegistration5Output1.png
  Execute $<TARGET_FILE:ImageRegistration5>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbRotated10.png
    ${TEMP}/ImageRegistration5Output1.png
    ${TEMP}/ImageRegistration5DifferenceBefore1.png
    ${TEMP}/ImageRegistration5DifferenceAfter1.png
)
otb_add_test(NAME reTeImageRegistration5Cas2ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration5Output2.png
    ${TEMP}/ImageRegistration5Output2.png
  Execute $<TARGET_FILE:ImageRegistration5>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbR10X13Y17.png
    ${TEMP}/ImageRegistration5Output2.png
    ${TEMP}/ImageRegistration5DifferenceBefore2.png
    ${TEMP}/ImageRegistration5DifferenceAfter2.png
)
