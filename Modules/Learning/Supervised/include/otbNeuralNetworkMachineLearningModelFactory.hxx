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

#ifndef otbNeuralNetworkMachineLearningModelFactory_hxx
#define otbNeuralNetworkMachineLearningModelFactory_hxx

#include "otbNeuralNetworkMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbNeuralNetworkMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModelFactory<TInputValue, TOutputValue>::NeuralNetworkMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass      = std::string("otbNeuralNetworkMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(), subclass.c_str(), "Artificial Neural Network ML Model", 1,
                         itk::CreateObjectFunction<NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>>::New());
}

template <class TInputValue, class TOutputValue>
const char* NeuralNetworkMachineLearningModelFactory<TInputValue, TOutputValue>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char* NeuralNetworkMachineLearningModelFactory<TInputValue, TOutputValue>::GetDescription() const
{
  return "Artificial Neural Network machine learning model factory";
}

} // end namespace otb

#endif
