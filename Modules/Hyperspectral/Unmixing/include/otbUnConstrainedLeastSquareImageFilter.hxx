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

#ifndef otbUnConstrainedLeastSquareImageFilter_hxx
#define otbUnConstrainedLeastSquareImageFilter_hxx

#include "otbUnConstrainedLeastSquareImageFilter.h"

namespace otb
{
namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
size_t UnConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>::OutputSize(const std::array<size_t, 1>&) const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
void UnConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>::SetMatrix(const MatrixType& m)
{
  m_Svd.reset(new SVDType(m));
  m_Inv        = m_Svd->inverse();
  m_OutputSize = m.cols();
}

template <class TInput, class TOutput, class TPrecision>
typename UnConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>::OutputType UnConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>::
operator()(const InputType& in) const
{
  VectorType inVector(in.Size());
  for (unsigned int i = 0; i < in.GetSize(); ++i)
  {
    inVector[i] = in[i];
  }

  VectorType outVector = m_Inv * inVector;

  OutputType out(outVector.size());
  for (unsigned int i = 0; i < out.GetSize(); ++i)
  {
    out[i] = outVector[i];
  }

  return out;
}


} // end namespace Functor
} // end namespace otb

#endif
