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

#ifndef otbCompositeIndicesFunctor_h
#define otbCompositeIndicesFunctor_h

#include "otbWaterIndicesFunctor.h"
#include "otbBuiltUpIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"

#include <vector>
#include <stdexcept>

namespace otb {

namespace Functor {

template <typename TInput, typename TOuput>
class CompositeIndicesFunctor
{
public:
  using InputType  = itk::VariableLengthVector<TInput>;
  using OutputType = itk::VariableLengthVector<TOuput>;
  using IndiceType = RadiometricIndice<TInput,TOuput>;

  CompositeIndicesFunctor(std::vector<IndiceType*> & indices) 
    : m_Indices(indices)
  {}

  void operator()(OutputType & out, const InputType & in) const
  {
    size_t idx = 0;
    for(auto indice : m_Indices)
      {
      out[idx] = (*indice)(in);
      }
  }

  size_t OutputSize(...) const
  {
    return m_Indices.size();
  } 

private:
  std::vector<IndiceType*> m_Indices;
};

} // End namespace Functor

} // End namespace otb




#endif
