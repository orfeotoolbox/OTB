/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

template <TransformDirection TDirectionOfMapping, class TScalarType>
GeocentricTransform<TDirectionOfMapping, TScalarType>::GeocentricTransform() : Superclass(ParametersDimension)
{
  SpatialReference wgs84               = SpatialReference::FromWGS84();
  SpatialReference ecefSpatialReference = SpatialReference::FromDescription("EPSG:4978");

#if GDAL_VERSION_NUM >= 3000000
  wgs84.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  ecefSpatialReference.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

  if (DirectionOfMapping == TransformDirection::INVERSE)
  {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(ecefSpatialReference, wgs84));

    if (newMapProjection)
      m_MapProjection.swap(newMapProjection);
  }
  else
  {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(wgs84, ecefSpatialReference));

    if (newMapProjection)
      m_MapProjection.swap(newMapProjection);
  }

}

template <TransformDirection TDirectionOfMapping, class TScalarType>
typename GeocentricTransform<TDirectionOfMapping, TScalarType>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType>::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  std::tie(outputPoint[0], outputPoint[1], outputPoint[2]) = m_MapProjection->Transform(std::make_tuple(point[0], point[1], point[2]));

  return outputPoint;
}

} // namespace otb
#endif
