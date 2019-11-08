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

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Learning)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)

if(OTBLibSVM_LOADED)
# ------- SVMImageEstimatorClassificationMultiExampleTest----------

otb_add_test(NAME leTeSVMImageEstimatorClassificationMultiExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
  Execute $<TARGET_FILE:SVMImageEstimatorClassificationMultiExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${INPUTDATA}/ROI_mask_multi.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.png
)

endif()
# ------- GenerateTrainingImageExampleTest----------

otb_add_test(NAME leTeGenerateTrainingImageExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROI_QB_MUL_4_training.png
    ${TEMP}/ROI_QB_MUL_4_training.png
  Execute $<TARGET_FILE:GenerateTrainingImageExample>
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${INPUTDATA}/LearningROIs.txt
    ${TEMP}/ROI_QB_MUL_4_training.png
)

# ------- SOMExampleTest----------

otb_add_test(NAME leTeSOMExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/ROI_QB_MUL_SOM.png
    ${TEMP}/ROI_QB_MUL_SOM.png
    ${BASELINE}/ROI_QB_MUL_SOMACT.png
    ${TEMP}/ROI_QB_MUL_SOMACT.png
  Execute $<TARGET_FILE:SOMExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${TEMP}/ROI_QB_MUL_SOM.png
    ${TEMP}/ROI_QB_MUL_SOMACT.png
    4 4 4 4 15 1.0 0.1 128
)

# ------- SOMClassifierExampleTest----------

otb_add_test(NAME leTeSOMClassifierExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ROI_QB_MUL_extractSOMCLASS.tif
    ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
  Execute $<TARGET_FILE:SOMClassifierExample>
    ${INPUTDATA}/ROI_QB_MUL_1_extract.tif
    ${INPUTDATA}/ROI_QB_MUL_extractSOM.tif
    ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
)

# ------- SEMModelEstimatorExampleTest----------

otb_add_test(NAME leTeSEMModelEstimatorExampleTest COMMAND ${OTB_TEST_DRIVER}
#        --compare-image ${NOTOL}
#        ${BASELINE}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
#        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
  Execute $<TARGET_FILE:SEMModelEstimatorExample>
    ${INPUTDATA}/ROI_QB_MUL_1.tif
    ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
    4 40 5
)
