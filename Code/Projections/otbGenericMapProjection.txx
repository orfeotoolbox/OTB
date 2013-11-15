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
#ifndef __otbGenericMapProjection_txx
#define __otbGenericMapProjection_txx

#include "otbGenericMapProjection.h"
#include "otbMacro.h"

namespace otb
{

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GenericMapProjection() : Superclass(ParametersDimension)
{
  m_MapProjection = MapProjectionAdapter::New();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::~GenericMapProjection()
{
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
const MapProjectionAdapter*
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetMapProjection() const
{
  return m_MapProjection;
}


template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
std::string
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetWkt()
{
  return m_MapProjection->GetWkt();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetWkt(const std::string& projectionRefWkt)
{
  m_MapProjection->SetWkt(projectionRefWkt);
  this->Modified();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
bool
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::InstanciateProjection()
{
  return m_MapProjection->InstanciateProjection();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
typename GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  if (DirectionOfMapping == TransformDirection::INVERSE)
    {

    double lon, lat, h;
    double z = 0.0;
    if (InputPointType::PointDimension == 3) z = point[2];

    m_MapProjection->InverseTransform(point[0], point[1], z, lon, lat, h);

    outputPoint[0] = lon;
    outputPoint[1] = lat;
    if (OutputPointType::PointDimension == 3) outputPoint[2] = h;

    }
  if (DirectionOfMapping == TransformDirection::FORWARD)
    {

    double x, y, z;
    double h = 0.0;
    if (InputPointType::PointDimension == 3) h = point[2];
    m_MapProjection->ForwardTransform(point[0], point[1], h, x, y, z);
    outputPoint[0] = x;
    outputPoint[1] = y;
    if (OutputPointType::PointDimension == 3) outputPoint[2] = z;

    }

  return outputPoint;
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::PrintMap() const
{
  m_MapProjection->PrintMap();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
bool
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::IsProjectionDefined() const
{
  return (m_MapProjection->GetMapProjection() != NULL);
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
  void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::SetParameter(const std::string& key, const std::string& value)
{
  m_MapProjection->SetParameter(key, value);
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
std::string
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GetParameter(const std::string& key) const
{
  return m_MapProjection->GetParameter(key);
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType, unsigned int NInputDimensions,
    unsigned int NOutputDimensions>
void
GenericMapProjection<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "ProjectionRefWkt: " << m_MapProjection->GetWkt() << std::endl;
}

} // namespace otb

#endif
