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

#ifndef otbISRAUnmixingImageFilter_hxx
#define otbISRAUnmixingImageFilter_hxx

#include "otbISRAUnmixingImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::ISRAUnmixingFunctor() : m_OutputSize(0), m_MaxIteration(100)
{
}

template <class TInput, class TOutput, class TPrecision>
size_t ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::OutputSize(const std::array<size_t, 1>&) const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
void ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::SetEndmembersMatrix(const MatrixType& U)
{
  m_U          = U;
  m_OutputSize = m_U.cols();
  m_Svd.reset(new SVDType(m_U));
}


template <class TInput, class TOutput, class TPrecision>
const typename ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::MatrixType& ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::GetEndmembersMatrix() const
{
  return m_U;
}

template <class TInput, class TOutput, class TPrecision>
typename ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::OutputType ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::operator()(const InputType& in) const
{
  // TODO : support different types between input and output ?
  VectorType inVector(in.Size());
  for (unsigned int i = 0; i < in.GetSize(); ++i)
  {
    inVector[i] = in[i];
  }

  // Initialize with Unconstrained Least Square solution
  VectorType outVector = m_Svd->solve(inVector);

  unsigned int nbEndmembers = m_OutputSize;
  unsigned int nbBands      = in.Size();

  // Apply ISRA iterations
  for (unsigned int i = 0; i < m_MaxIteration; ++i)
  {

    // Use a temporary storage since it is used
    // inside the iterations
    VectorType outVectorNew = outVector;
    for (unsigned int e = 0; e < nbEndmembers; ++e)
    {
      PrecisionType numerator   = 0;
      PrecisionType denominator = 0;

      for (unsigned int b = 0; b < nbBands; ++b)
      {
        numerator += in[b] * m_U(b, e);

        PrecisionType dot = 0;
        for (unsigned int s = 0; s < nbEndmembers; ++s)
        {
          // Use outVector from previous iteration here
          dot += m_U(b, s) * outVector[s];
        }
        denominator += dot * m_U(b, e);
      }

      outVectorNew[e] *= (numerator / denominator);
    }

    // Prepare for next iteration
    outVector = outVectorNew;
  }

  OutputType out(outVector.size());
  for (unsigned int i = 0; i < out.GetSize(); ++i)
  {
    out[i] = outVector[i];
  }
  return out;
}

} // end namespace functor
} // end namespace otb

#endif
