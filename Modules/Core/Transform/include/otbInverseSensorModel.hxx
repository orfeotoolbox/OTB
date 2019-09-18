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

#ifndef otbInverseSensorModel_hxx
#define otbInverseSensorModel_hxx

#include "otbInverseSensorModel.h"
#include "otbMacro.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::~InverseSensorModel()
{
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(const InputPointType& point) const
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
void InverseSensorModel<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb

#endif
