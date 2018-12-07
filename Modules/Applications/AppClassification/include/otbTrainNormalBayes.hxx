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

#ifndef otbTrainNormalBayes_hxx
#define otbTrainNormalBayes_hxx
#include "otbLearningApplicationBase.h"
#include "otbNormalBayesMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::InitNormalBayesParams()
  {
    AddChoice("classifier.bayes", "Normal Bayes classifier");
    SetParameterDescription("classifier.bayes", "http://docs.opencv.org/modules/ml/doc/normal_bayes_classifier.html");

  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainNormalBayes(typename ListSampleType::Pointer trainingListSample,
                     typename TargetListSampleType::Pointer trainingLabeledListSample,
                     std::string modelPath)
  {
    typedef otb::NormalBayesMachineLearningModel<InputValueType, OutputValueType> NormalBayesType;
    typename NormalBayesType::Pointer classifier = NormalBayesType::New();
    classifier->SetRegressionMode(this->m_RegressionFlag);
    classifier->SetInputListSample(trainingListSample);
    classifier->SetTargetListSample(trainingLabeledListSample);
    classifier->Train();
    classifier->Save(modelPath);
  }

} //end namespace wrapper
} //end namespace otb

#endif
