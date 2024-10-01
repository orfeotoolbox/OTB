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
#ifndef otbTrainDimensionalityReductionApplicationBase_hxx
#define otbTrainDimensionalityReductionApplicationBase_hxx

#include "otbTrainDimensionalityReductionApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::TrainDimensionalityReductionApplicationBase()
{
}

template <class TInputValue, class TOutputValue>
TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::~TrainDimensionalityReductionApplicationBase()
{
  ModelFactoryType::CleanFactories();
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::DoInit()
{
  AddDocTag(Tags::Learning);

  // main choice parameter that will contain all dimensionality reduction options
  AddParameter(ParameterType_Choice, "algorithm", "algorithm to use for the training");
  SetParameterDescription("algorithm",
                          "Choice of the dimensionality reduction "
                          "algorithm to use for the training.");

  InitSOMParams();

#ifdef OTB_USE_SHARK
  InitAutoencoderParams();
  InitPCAParams();
#endif
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::Reduce(typename ListSampleType::Pointer /*validationListSample*/,
                                                                                    std::string /*modelPath*/)
{
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::Train(typename ListSampleType::Pointer trainingListSample, std::string modelPath)
{
  // get the name of the chosen machine learning model
  const std::string modelName = GetParameterString("algorithm");
  // call specific train function

  if (modelName == "som")
  {
    BeforeTrainSOM(trainingListSample, modelPath);
  }

  if (modelName == "autoencoder")
  {
#ifdef OTB_USE_SHARK
    BeforeTrainAutoencoder(trainingListSample, modelPath);
#else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
#endif
  }

  if (modelName == "pca")
  {
#ifdef OTB_USE_SHARK
    TrainPCA(trainingListSample, modelPath);
#else
    otbAppLogFATAL("Module SharkLearning is not installed. You should consider turning OTB_USE_SHARK on during cmake configuration.");
#endif
  }
}

} // end of namespace Wrapper
} // end of namespace otb

#endif
