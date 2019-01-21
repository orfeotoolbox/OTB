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

otb_add_test(NAME leTvANNMachineLearningModel COMMAND otbSupervisedTestDriver
  otbANNMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/ann_model.txt
  )

# ------------------ Regression tests --------------------
otb_add_test(NAME leTvANNMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbNeuralNetworkRegressionTests
  )

otb_add_test(NAME leTvSVMMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbSVMRegressionTests
  )

otb_add_test(NAME leTvDecisionTreeMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbDecisionTreeRegressionTests
  )
if(NOT OTB_OPENCV_3)
otb_add_test(NAME leTvGradientBoostedTreeMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbGradientBoostedTreeRegressionTests
  )
endif()

otb_add_test(NAME leTvKNearestNeighborsMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbKNearestNeighborsRegressionTests
  )

otb_add_test(NAME leTvRandomForestsMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbRandomForestsRegressionTests
  )
# --------------------------------------------------------------

otb_add_test(NAME leTvSVMMachineLearningRegressionModel COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningRegressionModel
  ${INPUTDATA}/abalone.scale
  ${TEMP}/svm_model_regression.txt
  )

otb_add_test(NAME leTvSVMMachineLearningModel COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/svm_model.txt
  )

otb_add_test(NAME leTvNormalBayesMachineLearningModel COMMAND otbSupervisedTestDriver
  otbNormalBayesMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/normalbayes_model.txt
  )

if(NOT OTB_OPENCV_3)
  otb_add_test(NAME leTvGradientBoostedTreeMachineLearningModel COMMAND otbSupervisedTestDriver
    otbGradientBoostedTreeMachineLearningModel
    ${INPUTDATA}/letter_light.scale
    ${TEMP}/gbt_model.txt
    )
endif()

otb_add_test(NAME leTvRandomForestsMachineLearningModel COMMAND otbSupervisedTestDriver
  otbRandomForestsMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/rf_model.txt
  )

otb_add_test(NAME leTvKNearestNeighborsMachineLearningModel COMMAND otbSupervisedTestDriver
  otbKNearestNeighborsMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/knn_model.txt
  )

otb_add_test(NAME leTvDecisionTreeMachineLearningModel COMMAND otbSupervisedTestDriver
  otbDecisionTreeMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/decisiontree_model.txt
  )

otb_add_test(NAME leTvBoostMachineLearningModel COMMAND otbSupervisedTestDriver
  otbBoostMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/boost_model.txt
  )

otb_add_test(NAME leTvImageClassificationFilterSVM COMMAND otbSupervisedTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leImageClassificationFilterSVMOutput.tif
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  )

otb_add_test(NAME leTvDecisionTreeMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbDecisionTreeMachineLearningModelCanRead
  ${TEMP}/decisiontree_model.txt
  )
set_property(TEST leTvDecisionTreeMachineLearningModelCanRead APPEND PROPERTY DEPENDS leTvDecisionTreeMachineLearningModel)

if(NOT OTB_OPENCV_3)
  otb_add_test(NAME leTvGradientBoostedTreeMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
    otbGradientBoostedTreeMachineLearningModelCanRead
    ${TEMP}/gbt_model.txt
    )
  set_property(TEST leTvGradientBoostedTreeMachineLearningModelCanRead PROPERTY DEPENDS leTvGradientBoostedTreeMachineLearningModel)
endif()

otb_add_test(NAME leTvNormalBayesMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbNormalBayesMachineLearningModelCanRead
  ${TEMP}/normalbayes_model.txt
  )
set_property(TEST leTvNormalBayesMachineLearningModelCanRead PROPERTY DEPENDS leTvNormalBayesMachineLearningModel)

otb_add_test(NAME leTvANNMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbNeuralNetworkMachineLearningModelCanRead
  ${TEMP}/ann_model.txt
  )
set_property(TEST leTvANNMachineLearningModelCanRead PROPERTY DEPENDS leTvANNMachineLearningModel)

otb_add_test(NAME leTvSVMMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningModelCanRead
  ${TEMP}/svm_model.txt
  )
set_property(TEST leTvSVMMachineLearningModelCanRead PROPERTY DEPENDS leTvSVMMachineLearningModel)

otb_add_test(NAME leTvBoostMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbBoostMachineLearningModelCanRead
  ${TEMP}/boost_model.txt
  )
set_property(TEST leTvBoostMachineLearningModelCanRead PROPERTY DEPENDS leTvBoostMachineLearningModel)

otb_add_test(NAME leTvRandomForestsMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbRandomForestsMachineLearningModelCanRead
  ${TEMP}/rf_model.txt
  )
set_property(TEST leTvRandomForestsMachineLearningModelCanRead PROPERTY DEPENDS leTvRandomForestsMachineLearningModel)

otb_add_test(NAME leTvKNNMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbKNNMachineLearningModelCanRead
  ${TEMP}/knn_model.txt
  )
set_property(TEST leTvKNNMachineLearningModelCanRead PROPERTY DEPENDS leTvKNearestNeighborsMachineLearningModel)
