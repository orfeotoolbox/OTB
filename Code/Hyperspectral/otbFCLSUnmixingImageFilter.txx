/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbFCLSUnmixingImageFilter_txx
#define __otbFCLSUnmixingImageFilter_txx

#include "otbFCLSUnmixingImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::FCLSUnmixingFunctor()
 : m_OutputSize(0),
   m_MaxIteration(10)
{
}

template <class TInput, class TOutput, class TPrecision>
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::~FCLSUnmixingFunctor()
{
}

template <class TInput, class TOutput, class TPrecision>
unsigned int
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
bool
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::operator != (const Self& other) const
{
  return true;
}

template <class TInput, class TOutput, class TPrecision>
bool
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::operator == (const Self& other) const
{
  return false;
}

template <class TInput, class TOutput, class TPrecision>
void
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::SetEndmembersMatrix(const MatrixType& U)
{
  const double Delta = 1.0E-6;
  m_U = U;
  m_N.set_size(U.rows() + 1, U.cols());
  for (unsigned int r = 0; r < U.rows(); ++r)
    {
    m_N.set_row(r, U.get_row(r));
    }
  m_N.set_row(U.rows(), 1.0E6);

  m_Nt = m_N.transpose();

  MatrixType NtN = m_Nt * m_N;
  m_NtNinv = SVDType(NtN).inverse();

  m_OutputSize = m_N.cols();
  m_Svd.reset( new SVDType(m_U) );
}


template <class TInput, class TOutput, class TPrecision>
const typename FCLSUnmixingFunctor<TInput, TOutput, TPrecision>::MatrixType&
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
::GetEndmembersMatrix() const
{
  return m_U;
}

template <class TInput, class TOutput, class TPrecision>
typename FCLSUnmixingFunctor<TInput, TOutput, TPrecision>::OutputType
FCLSUnmixingFunctor<TInput, TOutput, TPrecision>
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
  unsigned int nbBands = in.Size();

  // Apply FCLS iterations
  const double Delta = 1.0E-6;
  VectorType s(nbBands + 1);
  for (unsigned int r = 0; r < nbBands; ++r)
    {
    s[r] = inVector[r];
    }
  s[nbBands] = 1.0E6;

  VectorType lambda(nbEndmembers);
  VectorType fclsVector = uclsVector;
  VectorType correction(uclsVector.size());
  for (unsigned int i = 0; i < m_MaxIteration; ++i)
    {
    // Error in original paper : divergence
    // lambda = m_Nt * (s - m_N * fclsVector);
    lambda = m_Nt * (m_N * fclsVector - s);
    correction = m_NtNinv * lambda;
    fclsVector -= correction;
    }

  OutputType out(fclsVector.size());
  for (unsigned int i = 0; i < out.GetSize(); ++i )
    {
    out[i] = fclsVector[i];
    }
  return out;
}

}

template <class TInputImage, class TOutputImage, class TPrecision>
FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::FCLSUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::~FCLSUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::SetEndmembersMatrix(const MatrixType& m)
{
  this->GetFunctor().SetEndmembersMatrix(m);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
const typename FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>::MatrixType&
FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::GetEndmembersMatrix() const
{
  return this->GetFunctor().GetEndmembersMatrix();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
FCLSUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
