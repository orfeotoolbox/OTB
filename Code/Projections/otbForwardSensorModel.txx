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
#ifndef __otbForwardSensorModel_txx
#define __otbForwardSensorModel_txx

#include "otbForwardSensorModel.h"

#include "otbMacro.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::ForwardSensorModel()
{}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::~ForwardSensorModel()
{}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  double x = point[0];
  double y = point[1];

  double lon, lat, h;

  if (InputPointType::PointDimension == 3)
    {
    double z = point[2];

    this->m_Model->ForwardTransformPoint(x, y, z, lon, lat, h);
    }
  else
    {
    this->m_Model->ForwardTransformPoint(x, y, lon, lat, h);
    }

  OutputPointType outputPoint;
  outputPoint[0] = lon;
  outputPoint[1] = lat;

  if (OutputPointType::PointDimension == 3)
    {
    outputPoint[2] = h;
    }
  return outputPoint;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
ForwardSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
