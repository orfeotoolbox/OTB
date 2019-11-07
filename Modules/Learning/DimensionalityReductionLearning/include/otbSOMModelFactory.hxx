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
#ifndef otbSOMFactory_hxx
#define otbSOMFactory_hxx

#include "otbSOMModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbSOMModel.h"
#include "itkVersion.h"

namespace otb
{
template <class TInputValue, class TOutputValue, unsigned int MapDimension>
SOMModelFactory<TInputValue, TOutputValue, MapDimension>::SOMModelFactory()
{
  std::string classOverride = std::string("DimensionalityReductionModel");
  std::string subclass      = std::string("SOMModel");

  this->RegisterOverride(classOverride.c_str(), subclass.c_str(), "SOM DR Model", 1, itk::CreateObjectFunction<SOMModel<TInputValue, MapDimension>>::New());
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
SOMModelFactory<TInputValue, TOutputValue, MapDimension>::~SOMModelFactory()
{
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
const char* SOMModelFactory<TInputValue, TOutputValue, MapDimension>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue, unsigned int MapDimension>
const char* SOMModelFactory<TInputValue, TOutputValue, MapDimension>::GetDescription() const
{
  return "SOM model factory";
}

} // end namespace otb

#endif
