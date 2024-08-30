/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbFlexibleDistanceWithMissingValue_hxx
#define otbFlexibleDistanceWithMissingValue_hxx

#include "otbFlexibleDistanceWithMissingValue.h"
#include "itkNumericTraits.h"

namespace otb
{

namespace Statistics
{

template <class TVector>
inline double FlexibleDistanceWithMissingValue<TVector>::Evaluate(const TVector& x1, const TVector& x2) const
{
  if (IsEuclidean())
    return Superclass::Evaluate(x1, x2);

  if (itk::NumericTraits<TVector>::GetLength(x1) != itk::NumericTraits<TVector>::GetLength(x2))
  {
    itkExceptionMacro(<< "Vector lengths must be equal.");
  }

  double temp, distance = itk::NumericTraits<double>::Zero;

  for (unsigned int i = 0; i < x1.Size(); ++i)
  {
    if (!this->IsMissingValue(x1[i]) && !this->IsMissingValue(x2[i]))
    {
      temp = std::pow(std::abs(std::pow(x1[i], this->Alpha) - std::pow(x2[i], this->Alpha)), this->Beta);
      distance += temp;
    }
  }

  return distance;
}

template <class TVector>
inline double FlexibleDistanceWithMissingValue<TVector>::Evaluate(const TVector& x) const
{
  if (IsEuclidean())
    return Superclass::Evaluate(x);

  MeasurementVectorSizeType measurementVectorSize = this->GetMeasurementVectorSize();

  if (measurementVectorSize == 0)
  {
    itkExceptionMacro(<< "Please set the MeasurementVectorSize first");
  }

  itk::Statistics::MeasurementVectorTraits::Assert(this->GetOrigin(), measurementVectorSize,
                                                   "EuclideanDistanceMetric::Evaluate Origin and input vector have different lengths");

  double temp, distance = itk::NumericTraits<double>::Zero;

  for (unsigned int i = 0; i < measurementVectorSize; ++i)
  {
    if (!this->IsMissingValue(this->GetOrigin()[i]) && !this->IsMissingValue(x[i]))
    {
      temp = std::pow(std::abs(std::pow(this->GetOrigin()[i], this->Alpha) - std::pow(x[i], this->Alpha)), this->Beta);
      distance += temp;
    }
  }

  return distance;
}

template <class TVector>
inline double FlexibleDistanceWithMissingValue<TVector>::Evaluate(const ValueType& a, const ValueType& b) const
{
  if (IsEuclidean())
    return Superclass::Evaluate(a, b);

  // FIXME throw NaN exception instead of returning 0. ??
  if (this->IsMissingValue(a) || this->IsMissingValue(b))
    return 0.0;

  double temp = std::pow(std::abs(std::pow(a, this->Alpha) - std::pow(b, this->Alpha)), this->Beta);
  return temp;
}

template <class TVector>
void FlexibleDistanceWithMissingValue<TVector>::SetAlphaBeta(double a, double b)
{
  Alpha = a;
  Beta  = b;
}

template <class TVector>
bool FlexibleDistanceWithMissingValue<TVector>::IsEuclidean()
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
