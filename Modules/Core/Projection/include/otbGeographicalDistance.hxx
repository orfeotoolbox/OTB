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

#ifndef otbGeographicalDistance_hxx
#define otbGeographicalDistance_hxx

#include "otbGeographicalDistance.h"
#include "otbMath.h"

namespace otb
{

template <class TVector>
GeographicalDistance<TVector>::GeographicalDistance() : m_EarthRadius(6371000)
{
}


template <class TVector>
double GeographicalDistance<TVector>::Evaluate(const VectorType& x) const
{
  // First check if vector length is sufficient
  if (x.Size() < 2)
    itkExceptionMacro(<< "Vector length must be at least 2 to compute geographical distance.");

  // Call evaluate implementation with the first point being the
  // origin
  VectorType origin(x);
  origin[0] = this->GetOrigin()[0];
  origin[1] = this->GetOrigin()[1];

  return this->Evaluate(origin, x);
}

template <class TVector>
double GeographicalDistance<TVector>::Evaluate(const VectorType& x, const VectorType& y) const
{
  // First check if vector length is sufficient
  if (x.Size() < 2 || y.Size() < 2)
    itkExceptionMacro(<< "Vector length must be at least 2 to compute geographical distance.");

  // Build some const variables
  const double One     = itk::NumericTraits<double>::One;
  const double Two     = One + One;
  const double Deg2Rad = CONST_PI / 180.;

  // Compute latitude and longitude differences
  double dLat = (std::fabs(x[1] - y[1])) * Deg2Rad;
  double dLon = (std::fabs(x[0] - y[0])) * Deg2Rad;

  // Compute dx in meters
  double a = std::sin(dLat / Two) * std::sin(dLat / Two) + std::cos(y[1] * Deg2Rad) * std::cos(x[1] * Deg2Rad) * std::sin(dLon / Two) * std::sin(dLon / Two);
  double c = Two * std::atan2(std::sqrt(a), std::sqrt(One - a));
  double d = m_EarthRadius * c;

  // Return result
  return d;
}

template <class TVector>
void GeographicalDistance<TVector>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Earth radius
  os << indent << "Earth radius: " << m_EarthRadius << std::endl;
}
} // End namespace otb

#endif
