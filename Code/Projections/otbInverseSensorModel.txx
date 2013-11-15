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
#ifndef __otbInverseSensorModel_txx
#define __otbInverseSensorModel_txx

#include "otbInverseSensorModel.h"
#include "otbMacro.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::~InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::TransformPoint(const InputPointType& point) const
{
  double lon = point[0];
  double lat = point[1];
  double x, y, z;

  if (InputPointType::PointDimension == 3)
    {
    double h = point[2];

    this->m_Model->InverseTransformPoint(lon, lat, h, x, y, z);
    }
  else
    {
    this->m_Model->InverseTransformPoint(lon, lat, x, y, z);
    }

  OutputPointType outputPoint;

  outputPoint[0] = x;
  outputPoint[1] = y;

  if (OutputPointType::PointDimension == 3)
    {
    outputPoint[2] = z;
    }

  return outputPoint;
}


template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
