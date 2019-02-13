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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Tutorials)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME  trTeTutorialsHelloWorld COMMAND ${OTB_TEST_DRIVER}
  Execute $<TARGET_FILE:HelloWorldOTB>
)

otb_add_test(NAME  trTeTutorialsPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/TutorialsPipelineOutput.png
    ${TEMP}/TutorialsPipelineOutput.png
  Execute $<TARGET_FILE:Pipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsFilteringPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/TutorialsFilteringPipelineOutput.png
    ${TEMP}/TutorialsFilteringPipelineOutput.png
  Execute $<TARGET_FILE:FilteringPipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsFilteringPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsScalingPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/TutorialsScalingPipelineOutput.png
    ${TEMP}/TutorialsScalingPipelineOutput.png
  Execute $<TARGET_FILE:ScalingPipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsScalingPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsMultispectralTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/MultispectralOutput1.tif
    ${TEMP}/MultispectralOutput1.tif
    ${BASELINE}/MultispectralOutput2.tif
    ${TEMP}/MultispectralOutput2.tif
  Execute $<TARGET_FILE:Multispectral>
    ${INPUTDATA}/qb_RoadExtract.tif
    ${TEMP}/MultispectralOutput1.tif
    ${TEMP}/MultispectralOutput2.tif
)

otb_add_test(NAME  trTeTutorialsOrthoFusionTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_2}
    ${BASELINE}/TutorialsOrthoFusionOutput.tif
    ${TEMP}/TutorialsOrthoFusionOutput.tif
  Execute $<TARGET_FILE:OrthoFusion>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
    ${TEMP}/TutorialsOrthoFusionOutput.tif
    31
    N
    375000
    4828100
    500
    500
    0.6
    -0.6
)
