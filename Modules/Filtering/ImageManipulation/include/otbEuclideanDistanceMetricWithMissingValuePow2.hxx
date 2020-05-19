/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbEuclideanDistanceMetricWithMissingValuePow2_hxx
#define otbEuclideanDistanceMetricWithMissingValuePow2_hxx

#include "otbEuclideanDistanceMetricWithMissingValuePow2.h"
#include "itkMeasurementVectorTraits.h"

namespace otb
{

namespace Statistics
{

template <class TVector>
inline double EuclideanDistanceMetricWithMissingValuePow2<TVector>::Evaluate(const TVector& x1, const TVector& x2) const
{
  if (itk::NumericTraits<TVector>::GetLength(x1) != itk::NumericTraits<TVector>::GetLength(x2))
  {
    itkExceptionMacro(<< "Vector lengths must be equal.");
  }

  double temp, distance = itk::NumericTraits<double>::Zero;

  for (unsigned int i = 0; i < x1.Size(); ++i)
  {
    if (!IsMissingValue(x1[i]) && !IsMissingValue(x2[i]))
    {
      temp = x1[i] - x2[i];
      distance += temp * temp;
    }
  }

  return distance;
}

template <class TVector>
inline double EuclideanDistanceMetricWithMissingValuePow2<TVector>::Evaluate(const TVector& x) const
{
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
    if (!IsMissingValue(this->GetOrigin()[i]) && !IsMissingValue(x[i]))
    {
      temp = this->GetOrigin()[i] - x[i];
      distance += temp * temp;
    }
  }

  return distance;
}

template <class TVector>
inline double EuclideanDistanceMetricWithMissingValuePow2<TVector>::Evaluate(const ValueType& a, const ValueType& b) const
{
  // FIXME throw NaN exception ??
  if (IsMissingValue(a) || IsMissingValue(b))
    return 0.0;

  double temp = a - b;
  return temp * temp;
}

template <class TVector>
/*static */
bool EuclideanDistanceMetricWithMissingValuePow2<TVector>::IsMissingValue(const ValueType& v)
{
  return static_cast<bool>(vnl_math_isnan(static_cast<double>(v)));
}

template <class TVector>
/* static */
void EuclideanDistanceMetricWithMissingValuePow2<TVector>::SetToMissingValue(ValueType& v)
{
  v = std::numeric_limits<ValueType>::signaling_NaN();
}

} // end namespace statistics

} // end namespace otb

#endif
