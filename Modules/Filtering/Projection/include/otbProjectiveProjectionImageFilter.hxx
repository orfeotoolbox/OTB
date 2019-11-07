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

#ifndef otbProjectiveProjectionImageFilter_hxx
#define otbProjectiveProjectionImageFilter_hxx

#include "otbProjectiveProjectionImageFilter.h"

namespace otb
{
namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
size_t ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::OutputSize(const std::array<size_t, 1>&) const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
const typename ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::InputType&
ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::GetProjectionDirection()
{
  return m_ProjectionDirection;
}

template <class TInput, class TOutput, class TPrecision>
void ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::SetProjectionDirection(const InputType& p)
{
  m_ProjectionDirection = p;
  m_OutputSize          = m_ProjectionDirection.Size();
}

template <class TInput, class TOutput, class TPrecision>
typename ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::OutputType ProjectiveProjectionFunctor<TInput, TOutput, TPrecision>::
operator()(const InputType& in)
{
  PrecisionType dotProduct = 0;
  for (unsigned int i = 0; i < in.Size(); ++i)
  {
    dotProduct += in[i] * m_ProjectionDirection[i];
  }

  OutputType projected(in.Size());
  for (unsigned int j = 0; j < in.Size(); ++j)
  {
    projected[j] = in[j] / dotProduct;
  }

  return projected;
}

} // end namespace Functor
} // end namespace otb

#endif
