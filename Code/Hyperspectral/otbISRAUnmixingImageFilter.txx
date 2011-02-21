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
#ifndef __otbISRAUnmixingImageFilter_txx
#define __otbISRAUnmixingImageFilter_txx

#include "otbISRAUnmixingImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::ISRAUnmixingFunctor()
 : m_OutputSize(0),
   m_MaxIteration(10)
{
}

template <class TInput, class TOutput, class TPrecision>
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::~ISRAUnmixingFunctor()
{
}


template <class TInput, class TOutput, class TPrecision>
unsigned int
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
bool
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::operator !=(const Self& other) const
{
  return true;
}

template <class TInput, class TOutput, class TPrecision>
bool
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::operator ==(const Self& other) const
{
  return false;
}

template <class TInput, class TOutput, class TPrecision>
void
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::SetEndmembersMatrix(const MatrixType& U)
{
  m_U = U;
  m_OutputSize = m_U.cols();
  m_Svd.reset( new SVDType(m_U) );
}


template <class TInput, class TOutput, class TPrecision>
const typename ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::MatrixType&
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::GetEndmembersMatrix() const
{
  return m_U;
}

template <class TInput, class TOutput, class TPrecision>
typename ISRAUnmixingFunctor<TInput, TOutput, TPrecision>::OutputType
ISRAUnmixingFunctor<TInput, TOutput, TPrecision>
::operator ()(const InputType& in) const
{
  // TODO : support different types between input and output ?
  VectorType inVector(in.GetDataPointer(), in.Size());
  VectorType outVector = m_Svd->solve(inVector);

  unsigned int nbEndmembers = m_OutputSize;
  unsigned int nbBands = in.Size();

  for (int i = 0; i < m_MaxIteration; ++i)
    {
    VectorType outVectorTmp = outVector;
    for (int e = 0; e < nbEndmembers; ++e)
      {
      PrecisionType numerator = 0;
      PrecisionType denominator = 0;

      for (int b = 0; b < nbBands; ++b)
        {
        numerator += in[b] * m_U(b, e);

        PrecisionType dot = 0;
        for (int s = 0; s < nbEndmembers; ++s)
          {
          dot += m_U(b, s) * outVectorTmp[s];
          }

        denominator += dot * m_U(b, e);
        }
      outVectorTmp[e] *= (numerator/denominator);
      }

    outVector = outVectorTmp;
    }

  return OutputType(outVector.data_block(), outVector.size());
}

}

template <class TInputImage, class TOutputImage, class TPrecision>
ISRAUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::ISRAUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
ISRAUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::~ISRAUnmixingImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
ISRAUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::SetEndmembersMatrix(const MatrixType& m)
{
  this->GetFunctor().SetEndmembersMatrix(m);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
const typename ISRAUnmixingImageFilter<TInputImage,TOutputImage,TPrecision>::MatrixType&
ISRAUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::GetEndmembersMatrix() const
{
  return this->GetFunctor().GetEndmembersMatrix();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
ISRAUnmixingImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
