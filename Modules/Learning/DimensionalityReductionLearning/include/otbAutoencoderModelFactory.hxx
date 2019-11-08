/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbAutoencoderModelFactory_hxx
#define otbAutoencoderModelFactory_hxx

#include "otbAutoencoderModelFactory.h"
#include "otbAutoencoderModel.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue, class NeuronType>
AutoencoderModelFactory<TInputValue, TOutputValue, NeuronType>::AutoencoderModelFactory()
{
  std::string classOverride = std::string("DimensionalityReductionModel");
  std::string subclass      = std::string("AutoencoderModel");

  this->RegisterOverride(classOverride.c_str(), subclass.c_str(), "Shark AE ML Model", 1,
                         itk::CreateObjectFunction<AutoencoderModel<TInputValue, NeuronType>>::New());
}

template <class TInputValue, class TOutputValue, class NeuronType>
AutoencoderModelFactory<TInputValue, TOutputValue, NeuronType>::~AutoencoderModelFactory()
{
}

template <class TInputValue, class TOutputValue, class NeuronType>
const char* AutoencoderModelFactory<TInputValue, TOutputValue, NeuronType>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue, class NeuronType>
const char* AutoencoderModelFactory<TInputValue, TOutputValue, NeuronType>::GetDescription() const
{
  return "Autoencoder model factory";
}

} // end namespace otb

#endif
