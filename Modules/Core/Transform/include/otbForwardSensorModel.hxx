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

#ifndef otbForwardSensorModel_hxx
#define otbForwardSensorModel_hxx

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
