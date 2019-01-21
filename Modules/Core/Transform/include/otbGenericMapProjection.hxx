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

#ifndef otbGenericMapProjection_hxx
#define otbGenericMapProjection_hxx

#include "otbGenericMapProjection.h"
#include "otbMacro.h"

namespace otb
{

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GenericMapProjection() : Superclass(ParametersDimension), m_MapProjection()
{}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::~GenericMapProjection()
{
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
std::string
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetWkt()
{
  if(m_MapProjection)
    {
    // Use partial template specialisation instead ?
    if(DirectionOfMapping == TransformDirection::FORWARD)
      {
      return m_MapProjection->GetTargetSpatialReference().ToWkt();
      }
    else
      {
      return m_MapProjection->GetSourceSpatialReference().ToWkt();
      }
    }
  return "";
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetWkt(const std::string& projectionRefWkt)
{
  SpatialReference wgs84 = SpatialReference::FromWGS84();
  SpatialReference wktSpatialReference = SpatialReference::FromDescription(projectionRefWkt);

  if(DirectionOfMapping == TransformDirection::INVERSE)
    {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(wktSpatialReference,wgs84));

    if(newMapProjection)
      m_MapProjection.swap(newMapProjection);
    }
  else
    {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(wgs84,wktSpatialReference));

    if(newMapProjection)
      m_MapProjection.swap(newMapProjection);
    }

  this->Modified();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
typename GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  // Can be collapsed
  if (DirectionOfMapping == TransformDirection::INVERSE)
    {
    double lon, lat, h;
    double z = 0.0;
    if (InputPointType::PointDimension == 3) z = point[2];

    std::tie(lon,lat,h) = m_MapProjection->Transform(std::make_tuple(point[0], point[1], z));

    outputPoint[0] = lon;
    outputPoint[1] = lat;
    if (OutputPointType::PointDimension == 3) outputPoint[2] = h;

    }
  if (DirectionOfMapping == TransformDirection::FORWARD)
    {

    double x, y, z;
    double h = 0.0;
    if (InputPointType::PointDimension == 3) h = point[2];
    std::tie(x,y,z) = m_MapProjection->Transform(std::make_tuple(point[0], point[1], h));
    outputPoint[0] = x;
    outputPoint[1] = y;
    if (OutputPointType::PointDimension == 3) outputPoint[2] = z;
    }

  return outputPoint;
}


template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
bool
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::IsProjectionDefined() const
{
  return m_MapProjection != nullptr;
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << *m_MapProjection.get() << std::endl;
}

} // namespace otb

#endif
