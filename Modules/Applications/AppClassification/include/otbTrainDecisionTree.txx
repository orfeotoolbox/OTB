/*=========================================================================
 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef __otbTrainDecisionTree_txx
#define __otbTrainDecisionTree_txx
#include "otbLearningApplicationBase.h"

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
  SetParameterInt("classifier.dt.max", 65535);
  SetParameterDescription(
      "classifier.dt.max", "The training algorithm attempts to split each node while its depth is smaller than the maximum "
      "possible depth of the tree. The actual depth may be smaller if the other termination criteria are met, and/or "
      "if the tree is pruned.");

  //MinSampleCount
  AddParameter(ParameterType_Int, "classifier.dt.min", "Minimum number of samples in each node");
  SetParameterInt("classifier.dt.min", 10);
  SetParameterDescription("classifier.dt.min", "If the number of samples in a node is smaller than this parameter, "
                          "then this node will not be split.");

  //RegressionAccuracy
  AddParameter(ParameterType_Float, "classifier.dt.ra", "Termination criteria for regression tree");
  SetParameterFloat("classifier.dt.ra", 0.01);
  SetParameterDescription("classifier.dt.min", "If all absolute differences between an estimated value in a node "
                          "and the values of the train samples in this node are smaller than this regression accuracy parameter, "
                          "then the node will not be split.");

  //UseSurrogates : don't need to be exposed !
  //AddParameter(ParameterType_Empty, "classifier.dt.sur", "Surrogate splits will be built");
  //SetParameterDescription("classifier.dt.sur","These splits allow working with missing data and compute variable importance correctly.");

  //MaxCategories
  AddParameter(ParameterType_Int, "classifier.dt.cat",
               "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split");
  SetParameterInt("classifier.dt.cat", 10);
  SetParameterDescription(
      "classifier.dt.cat",
      "Cluster possible values of a categorical variable into K <= cat clusters to find a suboptimal split.");

  //CVFolds
  AddParameter(ParameterType_Int, "classifier.dt.f", "K-fold cross-validations");
  SetParameterInt("classifier.dt.f", 10);
  SetParameterDescription(
      "classifier.dt.f", "If cv_folds > 1, then it prunes a tree with K-fold cross-validation where K is equal to cv_folds.");

  //Use1seRule
  AddParameter(ParameterType_Empty, "classifier.dt.r", "Set Use1seRule flag to false");
  SetParameterDescription(
      "classifier.dt.r",
      "If true, then a pruning will be harsher. This will make a tree more compact and more resistant to the training data noise but a bit less accurate.");

  //TruncatePrunedTree
  AddParameter(ParameterType_Empty, "classifier.dt.t", "Set TruncatePrunedTree flag to false");
  SetParameterDescription("classifier.dt.t", "If true, then pruned branches are physically removed from the tree.");

  //Priors are not exposed.

}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::TrainDecisionTree(typename ListSampleType::Pointer trainingListSample,
                    typename TargetListSampleType::Pointer trainingLabeledListSample,
                    std::string modelPath)
{
  typename DecisionTreeType::Pointer classifier = DecisionTreeType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);
  classifier->SetMaxDepth(GetParameterInt("classifier.dt.max"));
  classifier->SetMinSampleCount(GetParameterInt("classifier.dt.min"));
  classifier->SetRegressionAccuracy(GetParameterFloat("classifier.dt.ra"));
  classifier->SetMaxCategories(GetParameterInt("classifier.dt.cat"));
  classifier->SetCVFolds(GetParameterInt("classifier.dt.f"));
  if (IsParameterEnabled("classifier.dt.r"))
    {
    classifier->SetUse1seRule(false);
    }
  if (IsParameterEnabled("classifier.dt.t"))
    {
    classifier->SetTruncatePrunedTree(false);
    }
  classifier->Train();
  classifier->Save(modelPath);
}

} //end namespace wrapper
} //end namespace otb

#endif
