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

#ifndef otbNCLSUnmixingImageFilter_hxx
#define otbNCLSUnmixingImageFilter_hxx

#include "otbNCLSUnmixingImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::NCLSUnmixingFunctor()
 : m_OutputSize(0),
   m_MaxIteration(100)
{
}

template <class TInput, class TOutput, class TPrecision>
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::~NCLSUnmixingFunctor()
{
}

template <class TInput, class TOutput, class TPrecision>
unsigned int
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
bool
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::operator != (const Self& itkNotUsed(other)) const
{
  return true;
}

template <class TInput, class TOutput, class TPrecision>
bool
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::operator == (const Self& itkNotUsed(other)) const
{
  return false;
}

template <class TInput, class TOutput, class TPrecision>
void
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::SetEndmembersMatrix(const MatrixType& U)
{
  m_U = U;
  m_Ut = m_U.transpose();
  m_UtUinv = SVDType(m_Ut * m_U).inverse();
  m_OutputSize = m_U.cols();
  m_Svd.reset( new SVDType(m_U) );
}


template <class TInput, class TOutput, class TPrecision>
const typename NCLSUnmixingFunctor<TInput, TOutput, TPrecision>::MatrixType&
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::GetEndmembersMatrix() const
{
  return m_U;
}

template <class TInput, class TOutput, class TPrecision>
typename NCLSUnmixingFunctor<TInput, TOutput, TPrecision>::OutputType
NCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::operator ()(const InputType& in) const
{
  // TODO : support different types between input and output ?
  VectorType inVector(in.Size());
  for (unsigned int i = 0; i < in.GetSize(); ++i )
    {
    inVector[i] = in[i];
    }

  // Initialize with Unconstrained Least Square solution
  VectorType uclsVector = m_Svd->solve(inVector);

  unsigned int nbEndmembers = m_OutputSize;

  // Apply NCLS iterations
  VectorType lambda(nbEndmembers);
  VectorType nclsVector = uclsVector;
  VectorType correction(uclsVector.size());

  for (unsigned int i = 0; i < m_MaxIteration; ++i)
    {
    // Error in original paper : divergence
    // lambda = m_Ut * (inVector - m_U * nclsVector);
    lambda = m_Ut * (m_U * nclsVector - inVector);
    correction = m_UtUinv * lambda;
    nclsVector -= correction;
    }

  OutputType out(nclsVector.size());
  for (unsigned int i = 0; i < out.GetSize(); ++i )
    {
    out[i] = nclsVector[i];
    }
  return out;
}

}

template <class TInputImage, class TOutputImage, class TPrecision>
NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::NCLSUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::~NCLSUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::SetEndmembersMatrix(const MatrixType& m)
{
  this->GetFunctor().SetEndmembersMatrix(m);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
const typename NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>::MatrixType&
NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::GetEndmembersMatrix() const
{
  return this->GetFunctor().GetEndmembersMatrix();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
NCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
