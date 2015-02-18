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
#ifndef __otbTileMapTransform_txx
#define __otbTileMapTransform_txx

#include "otbTileMapTransform.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::TileMapTransform() : Superclass(ParametersDimension), m_Depth(0)
{}

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::~TileMapTransform()
{}

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  if (DirectionOfMapping == TransformDirection::INVERSE)
    {
    outputPoint[0] = static_cast<double>(point[0])/(1 << m_Depth)/256 *360.0-180.0;
    double y = static_cast<double>(point[1])/(1 << m_Depth)/256;
    double ex = exp(4*CONST_PI*(y-0.5));
    outputPoint[1] = -180.0/CONST_PI*asin((ex-1)/(ex+1));
    }

  if (DirectionOfMapping == TransformDirection::FORWARD)
    {
    double x = (180.0 + point[0]) / 360.0;
    double y = - point[1] * CONST_PI / 180; // convert to radians
    y = 0.5 * log((1+sin(y)) / (1 - sin(y)));
    y *= 1.0/(2 * CONST_PI); // scale factor from radians to normalized
    y += 0.5; // and make y range from 0 - 1

    outputPoint[0] = floor(x*pow(2., static_cast<double>(m_Depth))*256);
    outputPoint[1] = floor(y*pow(2., static_cast<double>(m_Depth))*256);
    }
  return outputPoint;
}

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::PrintMap() const
{}

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
void TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::SetLevel(unsigned int level)
{
  this->SetDepth(level);
}

template<TransformDirection::TransformationDirection TTransformDirection, class TScalarType,
    unsigned int NInputDimensions, unsigned int NOutputDimensions>
unsigned int TileMapTransform<TTransformDirection, TScalarType, NInputDimensions, NOutputDimensions>
::GetLevel() const
{
  return this->GetDepth();
}

} // namespace otb

#endif
