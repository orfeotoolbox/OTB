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
#ifndef __otbFullyConstrainedLeastSquareImageFilter_txx
#define __otbFullyConstrainedLeastSquareImageFilter_txx

#include "otbFullyConstrainedLeastSquareImageFilter.h"
#include <algorithm>

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput, class TPrecision>
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::FullyConstrainedLeastSquareFunctor()
 : m_OutputSize(0)
{
}

template <class TInput, class TOutput, class TPrecision>
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::~FullyConstrainedLeastSquareFunctor()
{
}


template <class TInput, class TOutput, class TPrecision>
unsigned int
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput, class TPrecision>
bool
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::operator !=(const Self& other) const
{
  return true;
}

template <class TInput, class TOutput, class TPrecision>
bool
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::operator ==(const Self& other) const
{
  return false;
}

template <class TInput, class TOutput, class TPrecision>
void
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::SetMatrix(const MatrixType& U)
{
  m_U = U;
  m_OutputSize = m_U.cols();
}

template <class TInput, class TOutput, class TPrecision>
typename FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>::OutputType
FullyConstrainedLeastSquareFunctor<TInput, TOutput, TPrecision>
::operator ()(const InputType& in) const
{
  // TODO : support different types between input and output ?
  VectorType inVector(in.GetDataPointer(), in.Size());
  VectorType out(m_OutputSize);

  MatrixType U = m_U;
  unsigned int count = m_U.cols();

  typedef std::vector<unsigned int> IndiciesType;
  IndiciesType indicies;
  for (unsigned int i = 0; i < m_OutputSize; ++i)
    {
    indicies.push_back(i);
    }

  for (count = m_U.cols(); count > 0; --count)
    {
     // First, solve the unconstrained least square system
     VectorType als_hat = SVDType(U).solve(inVector);

     MatrixType Z(1, count);
     Z.fill(itk::NumericTraits<PrecisionType>::One);
     MatrixType Zt = Z.transpose();
     MatrixType UtU = U.transpose() * U;
     MatrixType UtUinv = SVDType(UtU).inverse();
     MatrixType S = UtUinv * Zt;

     // Compute correction term needed for full additivity constraint
     PrecisionType correctionTerm = (S * SVDType( Z * S ).inverse())(0,0) * ((Z * als_hat)[0] - 1);

     // Remove the correction term from the unconstrained solution
     VectorType afcls_hat = als_hat - correctionTerm;

     // If everybody is positive, it is finished
     if ( std::count_if(afcls_hat.begin(), afcls_hat.end(), Self::IsNonNegative) == count )
       {
       out.fill(0);
       for (unsigned int j = 0; j < indicies.size(); ++j)
         {
         out[ indicies[j] ] = afcls_hat[ j ];
         }
       break;
       }

     // Multiply negative elements by their counterpart in the S vector
     // and find max(abs(afcls_hat)) for indicies where elements are negative

     unsigned int maxIdx = 0;
     PrecisionType maxi = itk::NumericTraits<PrecisionType>::min();

     for (unsigned int j = 0; j < afcls_hat.size(); ++j)
      {
      if (afcls_hat[j] < 0)
        {
        afcls_hat[j] = afcls_hat[j] / S(j,0);

        if (vcl_abs(afcls_hat[j]) > maxi)
          {
          maxi = vcl_abs(afcls_hat[j]);
          maxIdx = j;
          }
        }
      }

     // Remove column maxIdx from U
     MatrixType Unew(U.rows(), U.cols() - 1);
     for (unsigned int Ucol = 0, UcolNew = 0; Ucol < U.cols(); Ucol++)
       {
       if (Ucol != maxIdx)
         {
         Unew.set_column(UcolNew, U.get_column(Ucol));
         UcolNew++;
         }
       }

     // Remove maxIdx-th element from indicies
     IndiciesType::iterator itToRemove = indicies.begin() + maxIdx;
     indicies.erase(itToRemove);

     // Update U
     U = Unew;
    }

  // Convert to OutputType (vnl_vector -> VariableLengthVector)
  return OutputType(out.data_block(), out.size());
}

}

template <class TInputImage, class TOutputImage, class TPrecision>
FullyConstrainedLeastSquareImageFilter<TInputImage, TOutputImage, TPrecision>
::FullyConstrainedLeastSquareImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
FullyConstrainedLeastSquareImageFilter<TInputImage, TOutputImage, TPrecision>
::~FullyConstrainedLeastSquareImageFilter()
{
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
FullyConstrainedLeastSquareImageFilter<TInputImage, TOutputImage, TPrecision>
::SetMatrix(const MatrixType& m)
{
  this->GetFunctor().SetMatrix(m);
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void
FullyConstrainedLeastSquareImageFilter<TInputImage, TOutputImage, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
