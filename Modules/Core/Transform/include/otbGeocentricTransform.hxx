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

#ifndef otbGeocentricTransform_hxx
#define otbGeocentricTransform_hxx

#include "otbGeocentricTransform.h"

namespace otb
{

template <TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::GeocentricTransform() : Superclass(ParametersDimension)
{
  m_Ellipsoid = EllipsoidAdapter::New();
}

template <TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::~GeocentricTransform()
{
}

template <TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  if (DirectionOfMapping == TransformDirection::INVERSE)
  {
    m_Ellipsoid->XYZToLonLatHeight(point[0], point[1], point[2], outputPoint[0], outputPoint[1], outputPoint[2]);
  }
  if (DirectionOfMapping == TransformDirection::FORWARD)
  {
    m_Ellipsoid->LonLatHeightToXYZ(point[0], point[1], point[2], outputPoint[0], outputPoint[1], outputPoint[2]);
  }
  // To be completed
  return outputPoint;
}

} // namespace otb
#endif
