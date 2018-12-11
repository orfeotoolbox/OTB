/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbTrainRandomForests_hxx
#define otbTrainRandomForests_hxx
#include "otbLearningApplicationBase.h"
#include "otbRandomForestsMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitRandomForestsParams()
{
  AddChoice("classifier.rf", "Random forests classifier");
  SetParameterDescription("classifier.rf", "http://docs.opencv.org/modules/ml/doc/random_trees.html");
  //MaxDepth
  AddParameter(ParameterType_Int, "classifier.rf.max", "Maximum depth of the tree");
  SetParameterInt("classifier.rf.max",5);
  SetParameterDescription(
      "classifier.rf.max",
      "The depth of the tree. A low value will likely underfit and conversely a high value will likely overfit. "
      "The optimal value can be obtained using cross validation or other suitable methods.");

  //MinSampleCount
  AddParameter(ParameterType_Int, "classifier.rf.min", "Minimum number of samples in each node");
  SetParameterInt("classifier.rf.min",10);
  SetParameterDescription(
      "classifier.rf.min", "If the number of samples in a node is smaller than this parameter, "
      "then the node will not be split. A reasonable value is a small percentage of the total data e.g. 1 percent.");

  //RegressionAccuracy
  AddParameter(ParameterType_Float, "classifier.rf.ra", "Termination Criteria for regression tree");
  SetParameterFloat("classifier.rf.ra",0.);
  SetParameterDescription("classifier.rf.ra", "If all absolute differences between an estimated value in a node "
                          "and the values of the train samples in this node are smaller than this regression accuracy parameter, "
                          "then the node will not be split.");

  //MaxNumberOfCategories
  AddParameter(ParameterType_Int, "classifier.rf.cat",
               "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split");
  SetParameterInt("classifier.rf.cat",10);
  SetParameterDescription(
      "classifier.rf.cat",
      "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split.");

  //Priors are not exposed.

  //CalculateVariableImportance not exposed

  //MaxNumberOfVariables
  AddParameter(ParameterType_Int, "classifier.rf.var",
               "Size of the randomly selected subset of features at each tree node");
  SetParameterInt("classifier.rf.var",0);
  SetParameterDescription(
      "classifier.rf.var",
      "The size of the subset of features, randomly selected at each tree node, that are used to find the best split(s). "
      "If you set it to 0, then the size will be set to the square root of the total number of features.");

  //MaxNumberOfTrees
  AddParameter(ParameterType_Int, "classifier.rf.nbtrees",
               "Maximum number of trees in the forest");
  SetParameterInt("classifier.rf.nbtrees",100);
  SetParameterDescription(
      "classifier.rf.nbtrees",
      "The maximum number of trees in the forest. Typically, the more trees you have, the better the accuracy. "
      "However, the improvement in accuracy generally diminishes and reaches an asymptote for a certain number of trees. "
      "Also to keep in mind, increasing the number of trees increases the prediction time linearly.");

  //ForestAccuracy
  AddParameter(ParameterType_Float, "classifier.rf.acc",
               "Sufficient accuracy (OOB error)");
  SetParameterFloat("classifier.rf.acc",0.01);
  SetParameterDescription("classifier.rf.acc","Sufficient accuracy (OOB error).");


  //TerminationCriteria not exposed
}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::TrainRandomForests(typename ListSampleType::Pointer trainingListSample,
                     typename TargetListSampleType::Pointer trainingLabeledListSample,
                     std::string modelPath)
{
  typedef otb::RandomForestsMachineLearningModel<InputValueType, OutputValueType> RandomForestType;
  typename RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetMaxDepth(GetParameterInt("classifier.rf.max"));
  classifier->SetMinSampleCount(GetParameterInt("classifier.rf.min"));
  classifier->SetRegressionAccuracy(GetParameterFloat("classifier.rf.ra"));
  classifier->SetMaxNumberOfCategories(GetParameterInt("classifier.rf.cat"));
  classifier->SetMaxNumberOfVariables(GetParameterInt("classifier.rf.var"));
  classifier->SetMaxNumberOfTrees(GetParameterInt("classifier.rf.nbtrees"));
  classifier->SetForestAccuracy(GetParameterFloat("classifier.rf.acc"));

  classifier->Train();
  classifier->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
