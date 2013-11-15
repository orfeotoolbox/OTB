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
#ifndef __otbGeocentricTransform_txx
#define __otbGeocentricTransform_txx

#include "otbGeocentricTransform.h"

namespace otb
{

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
         unsigned int NInputDimensions, unsigned int NOutputDimensions>
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::GeocentricTransform() : Superclass(ParametersDimension)
{
  m_Ellipsoid = EllipsoidAdapter::New();
}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
         unsigned int NInputDimensions, unsigned int NOutputDimensions>
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::~GeocentricTransform()
{}

template<TransformDirection::TransformationDirection TDirectionOfMapping, class TScalarType,
         unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename GeocentricTransform<TDirectionOfMapping, TScalarType,
                             NInputDimensions, NOutputDimensions>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  if (DirectionOfMapping == TransformDirection::INVERSE)
    {
    m_Ellipsoid->XYZToLonLatHeight(point[0], point[1], point[2],
                                   outputPoint[0], outputPoint[1], outputPoint[2]);
    }
  if (DirectionOfMapping == TransformDirection::FORWARD)
    {
    m_Ellipsoid->LonLatHeightToXYZ(point[0], point[1], point[2],
                                   outputPoint[0], outputPoint[1], outputPoint[2]);
    }
  //To be completed
  return outputPoint;
}

} // namespace otb
#endif
