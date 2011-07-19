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
#ifndef __otbCLSPSTOUnmixingImageFilter_txx
#define __otbCLSPSTOUnmixingImageFilter_txx

#include "otbCLSPSTOUnmixingImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::CLSPSTOUnmixingFunctor()
 : m_OutputSize(0),
   m_MaxIteration(100)
{
}

template <class TInput, class TOutput, class TPrecision>
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::~CLSPSTOUnmixingFunctor()
{
}

template <class TInput, class TOutput, class TPrecision>
unsigned int
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
bool
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::operator !=(const Self& other) const
{
  return true;
}

template <class TInput, class TOutput, class TPrecision>
bool
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::operator ==(const Self& other) const
{
  return false;
}

template <class TInput, class TOutput, class TPrecision>
void
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::SetEndmembersMatrix(const MatrixType& U)
{
  m_U = U;
  m_OutputSize = m_U.cols();
  m_Svd.reset( new SVDType(m_U) );
}


template <class TInput, class TOutput, class TPrecision>
const typename CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>::MatrixType&
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::GetEndmembersMatrix() const
{
  return m_U;
}

template <class TInput, class TOutput, class TPrecision>
typename CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>::OutputType
CLSPSTOUnmixingFunctor<TInput, TOutput, TPrecision>
::operator ()(const InputType& in) const
{
  // TODO : support different types between input and output ?
  VectorType inVector(in.Size());
  for (int i = 0; i < in.GetSize(); i++ )
    {
    inVector[i] = in[i];
    }

  // Initialize with Unconstrained Least Square solution
  VectorType outVector = m_Svd->solve(inVector);

  unsigned int nbEndmembers = m_OutputSize;
  unsigned int nbBands = in.Size();

  // Apply CLSPSTO iterations
  for (int i = 0; i < m_MaxIteration; ++i)
    {
    // NOT IMPLEMENTED
    }

  OutputType out(outVector.size());
  for (int i = 0; i < out.GetSize(); i++ )
    {
    out[i] = outVector[i];
    }
  return out;
}

}

template <class TInputImage, class TOutputImage, class TPrecision>
CLSPSTOUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::CLSPSTOUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
CLSPSTOUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::~CLSPSTOUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
CLSPSTOUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::SetEndmembersMatrix(const MatrixType& m)
{
  this->GetFunctor().SetEndmembersMatrix(m);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
const typename CLSPSTOUnmixingImageFilter<TInputImage,TOutputImage,TPrecision>::MatrixType&
CLSPSTOUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::GetEndmembersMatrix() const
{
  return this->GetFunctor().GetEndmembersMatrix();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
CLSPSTOUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
