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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Markov)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME maTeMarkovClassification1ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/MarkovClassification1.png
    ${TEMP}/MarkovClassification1.png
  Execute $<TARGET_FILE:MarkovClassification1Example>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MarkovClassification1.png
    1.0
    20
    1.0
    1 #true to use always the same rand() sample, false otherwise
)

otb_add_test(NAME maTeMarkovClassification2ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/MarkovClassification2.png
    ${TEMP}/MarkovClassification2.png
  Execute $<TARGET_FILE:MarkovClassification2Example>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MarkovClassification2.png
    1.0
    5
    1 # true to use always the same rand() sample, false otherwise
)

otb_add_test(NAME maTeMarkovClassification3ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/MarkovClassification3.png
    ${TEMP}/MarkovClassification3.png
  Execute $<TARGET_FILE:MarkovClassification3Example>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MarkovClassification3.png
    ${TEMP}/MarkovClassification3_1.png
    1.0 20 1.0 1
)

otb_add_test(NAME maTeMarkovRestorationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/MarkovRestoration.png
    ${TEMP}/MarkovRestoration.png
  Execute $<TARGET_FILE:MarkovRestorationExample>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MarkovRestoration.png
    10.0
    30.0
    1.
    1 # true to use always the same rand() sample, false otherwise
)


otb_add_test(NAME maTeMarkovRegularizationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/MarkovRegularization-scaled.png
    ${TEMP}/MarkovRegularization-scaled.png
  Execute $<TARGET_FILE:MarkovRegularizationExample>
    ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
    ${TEMP}/MarkovRegularization.png
    ${TEMP}/MarkovRegularization-scaled.png
    0.2
    20
    0.0
    1 # true to use always the same rand() sample, false otherwis
)
