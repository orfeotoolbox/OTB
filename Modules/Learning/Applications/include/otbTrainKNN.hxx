/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTrainKNN_hxx
#define otbTrainKNN_hxx
#include "otbLearningApplicationBase.h"
#include "otbKNearestNeighborsMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitKNNParams()
{
  AddChoice("classifier.knn", "KNN classifier");
  SetParameterDescription("classifier.knn", "http://docs.opencv.org/modules/ml/doc/k_nearest_neighbors.html");

  // K parameter
  AddParameter(ParameterType_Int, "classifier.knn.k", "Number of Neighbors");
  SetParameterInt("classifier.knn.k", 32);
  SetParameterDescription("classifier.knn.k", "The number of neighbors to use.");

  if (this->m_RegressionFlag)
  {
    // Decision rule : mean / median
    AddParameter(ParameterType_Choice, "classifier.knn.rule", "Decision rule");
    SetParameterDescription("classifier.knn.rule", "Decision rule for regression output");

    AddChoice("classifier.knn.rule.mean", "Mean of neighbors values");
    SetParameterDescription("classifier.knn.rule.mean", "Returns the mean of neighbors values");

    AddChoice("classifier.knn.rule.median", "Median of neighbors values");
    SetParameterDescription("classifier.knn.rule.median", "Returns the median of neighbors values");
  }
}

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainKNN(typename ListSampleType::Pointer trainingListSample,
                                                                  typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath)
{
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType, OutputValueType> KNNType;
  typename KNNType::Pointer knnClassifier = KNNType::New();
  knnClassifier->SetRegressionMode(this->m_RegressionFlag);
  knnClassifier->SetInputListSample(trainingListSample);
  knnClassifier->SetTargetListSample(trainingLabeledListSample);
  knnClassifier->SetK(GetParameterInt("classifier.knn.k"));
  if (this->m_RegressionFlag)
  {
    std::string decision = this->GetParameterString("classifier.knn.rule");
    if (decision == "mean")
    {
      knnClassifier->SetDecisionRule(KNNType::KNN_MEAN);
    }
    else if (decision == "median")
    {
      knnClassifier->SetDecisionRule(KNNType::KNN_MEDIAN);
    }
  }

  knnClassifier->Train();
  knnClassifier->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
