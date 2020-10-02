#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

otb_add_test(NAME leTvLibSVMMachineLearningModel COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/libsvm_model.txt
  )
otb_add_test(NAME leTvImageClassificationFilterLibSVM COMMAND otbSupervisedTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  )

otb_add_test(NAME leTvLibSVMMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModelCanRead
  ${TEMP}/libsvm_model.txt
  )
set_property(TEST leTvLibSVMMachineLearningModelCanRead PROPERTY DEPENDS leTvLibSVMMachineLearningModel)

otb_add_test(NAME leTvLibSVMMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbLibSVMRegressionTests
  )

#otb_add_test(NAME obTvLabelMapSVMClassifier COMMAND otbSupervisedTestDriver
  #otbLabelMapClassifier
  #${INPUTDATA}/maur.tif
  #${INPUTDATA}/maur_labelled.tif
  #${TEMP}/obTvLabelMapSVMClassifierLabeledOutput.tif)

