/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbFlexibleDistanceWithMissingValue_txx
#define __otbFlexibleDistanceWithMissingValue_txx

#include "itkNumericTraits.h"

namespace otb {

namespace Statistics {

template<class TVector>
inline double
FlexibleDistanceWithMissingValue<TVector>
::Evaluate(const TVector& x1, const TVector& x2) const
{
  if (IsEuclidean()) return Superclass::Evaluate(x1, x2);

  if (itk::NumericTraits<TVector>::GetLength(x1) !=
      itk::NumericTraits<TVector>::GetLength(x2))
    {
    itkExceptionMacro(<< "Vector lengths must be equal.");
    }

  double temp, distance = itk::NumericTraits<double>::Zero;

  for (unsigned int i = 0; i < x1.Size(); ++i)
    {
    if (!this->IsMissingValue(x1[i]) && !this->IsMissingValue(x2[i]))
      {
      temp = vcl_pow(vcl_abs(vcl_pow(x1[i], this->Alpha) - vcl_pow(x2[i], this->Alpha)), this->Beta);
      distance += temp;
      }
    }

  return distance;
}

template<class TVector>
inline double
FlexibleDistanceWithMissingValue<TVector>
::Evaluate(const TVector& x) const
{
  if (IsEuclidean()) return Superclass::Evaluate(x);

  MeasurementVectorSizeType
    measurementVectorSize = this->GetMeasurementVectorSize();

  if (measurementVectorSize == 0)
    {
    itkExceptionMacro(<< "Please set the MeasurementVectorSize first");
    }

  itk::Statistics::MeasurementVectorTraits::Assert(this->GetOrigin(), measurementVectorSize,
                                       "EuclideanDistance::Evaluate Origin and input vector have different lengths");

  double temp, distance = itk::NumericTraits<double>::Zero;

  for (unsigned int i = 0; i < measurementVectorSize; ++i)
    {
    if (!this->IsMissingValue(this->GetOrigin()[i]) && !this->IsMissingValue(x[i]))
      {
      temp = vcl_pow(vcl_abs(vcl_pow(this->GetOrigin()[i], this->Alpha) - vcl_pow(x[i], this->Alpha)), this->Beta);
      distance += temp;
      }
    }

  return distance;
}

template<class TVector>
inline double
FlexibleDistanceWithMissingValue<TVector>
::Evaluate(const ValueType& a, const ValueType& b) const
{
  if (IsEuclidean()) return Superclass::Evaluate(a, b);

  // FIXME throw NaN exception instaed of returning 0. ??
  if (this->IsMissingValue(a) || this->IsMissingValue(b)) return 0.0;

  double temp = vcl_pow(vcl_abs(vcl_pow(a, this->Alpha) - vcl_pow(b, this->Alpha)), this->Beta);
  return temp;
}

template<class TVector>
void
FlexibleDistanceWithMissingValue<TVector>
::SetAlphaBeta(double a, double b)
{
  Alpha = a;
  Beta = b;
}

template<class TVector>
bool
FlexibleDistanceWithMissingValue<TVector>
::IsEuclidean()
{
  if ((Alpha == 1.0) && (Beta == 2.0))
    {
    return true;
    }
  else
    {
    return false;
    }
}

} // end namespace statistics
} // end namespace otb

#endif
