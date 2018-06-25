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

#ifndef otbTrainDecisionTree_hxx
#define otbTrainDecisionTree_hxx
#include "otbLearningApplicationBase.h"
#include "otbDecisionTreeMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitDecisionTreeParams()
{
  AddChoice("classifier.dt", "Decision Tree classifier");
  SetParameterDescription("classifier.dt",
    "This group of parameters allows setting Decision Tree classifier parameters. "
    "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/decision_trees.html}.");
  //MaxDepth
  AddParameter(ParameterType_Int, "classifier.dt.max", "Maximum depth of the tree");
#ifdef OTB_OPENCV_3
  SetParameterInt("classifier.dt.max",10);
#else
  SetParameterInt("classifier.dt.max",65535);
#endif
  SetParameterDescription("classifier.dt.max",
   "The training algorithm attempts to split each node while its depth is smaller "
   "than the maximum possible depth of the tree. The actual depth may be smaller "
   "if the other termination criteria are met, and/or if the tree is pruned.");

  //MinSampleCount
  AddParameter(ParameterType_Int, "classifier.dt.min", "Minimum number of samples in each node");
  SetParameterInt("classifier.dt.min",10);
  SetParameterDescription("classifier.dt.min",
    "If the number of samples in a node is smaller "
    "than this parameter, then this node will not be split.");

  //RegressionAccuracy
  AddParameter(ParameterType_Float, "classifier.dt.ra", "Termination criteria for regression tree");
  SetParameterFloat("classifier.dt.ra",0.01);
  SetParameterDescription("classifier.dt.ra",
    "If all absolute differences between an estimated value in a node "
    "and the values of the train samples in this node are smaller than this "
    "regression accuracy parameter, then the node will not be split further.");

  //UseSurrogates : don't need to be exposed !
  //AddParameter(ParameterType_Empty, "classifier.dt.sur", "Surrogate splits will be built");
  //SetParameterDescription("classifier.dt.sur","These splits allow working with missing data and compute variable importance correctly.");

  //MaxCategories
  AddParameter(ParameterType_Int, "classifier.dt.cat",
    "Cluster possible values of a categorical variable into K <= cat clusters to find a "
    "suboptimal split");
  SetParameterInt("classifier.dt.cat",10);
  SetParameterDescription("classifier.dt.cat",
      "Cluster possible values of a categorical variable into K <= cat clusters to find a "
      "suboptimal split.");

  //CVFolds
  AddParameter(ParameterType_Int, "classifier.dt.f", "K-fold cross-validations");
#ifdef OTB_OPENCV_3
  // disable cross validation by default (crash in opencv 3.2)
  SetParameterInt("classifier.dt.f",0);
#else
  SetParameterInt("classifier.dt.f",10);
#endif
  SetParameterDescription("classifier.dt.f",
    "If cv_folds > 1, then it prunes a tree with K-fold cross-validation where K "
    "is equal to cv_folds.");

  //Use1seRule
  AddParameter(ParameterType_Bool, "classifier.dt.r", "Set Use1seRule flag to false");
  SetParameterDescription("classifier.dt.r",
      "If true, then a pruning will be harsher. This will make a tree more compact and more "
      "resistant to the training data noise but a bit less accurate.");

  //TruncatePrunedTree
  AddParameter(ParameterType_Bool, "classifier.dt.t", "Set TruncatePrunedTree flag to false");
  SetParameterDescription("classifier.dt.t",
    "If true, then pruned branches are physically removed from the tree.");

  //Priors are not exposed.

}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::TrainDecisionTree(typename ListSampleType::Pointer trainingListSample,
                    typename TargetListSampleType::Pointer trainingLabeledListSample,
                    std::string modelPath)
{
  typedef otb::DecisionTreeMachineLearningModel<InputValueType, OutputValueType> DecisionTreeType;
  typename DecisionTreeType::Pointer classifier = DecisionTreeType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetMaxDepth(GetParameterInt("classifier.dt.max"));
  classifier->SetMinSampleCount(GetParameterInt("classifier.dt.min"));
  classifier->SetRegressionAccuracy(GetParameterFloat("classifier.dt.ra"));
  classifier->SetMaxCategories(GetParameterInt("classifier.dt.cat"));
  classifier->SetCVFolds(GetParameterInt("classifier.dt.f"));
  if (GetParameterInt("classifier.dt.r"))
    {
    classifier->SetUse1seRule(false);
    }
  if (GetParameterInt("classifier.dt.t"))
    {
    classifier->SetTruncatePrunedTree(false);
    }
  classifier->Train();
  classifier->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
