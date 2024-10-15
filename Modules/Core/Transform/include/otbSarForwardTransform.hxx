/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarForwardTransform_hxx
#define otbSarForwardTransform_hxx

#include "otbSarForwardTransform.h"

namespace otb
{
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
SarForwardTransform<TScalarType, NInputDimensions, NOutputDimensions>::SarForwardTransform() : Superclass(TransformDirection::FORWARD)
{}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename SarForwardTransform<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
SarForwardTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(const InputPointType& point) const
{
  SarSensorModel::Point2DType sensorPoint;
  SarSensorModel::Point3DType worldPoint;
  // from upper left corner to centered pixel convention
  sensorPoint[0] = static_cast<double>(point[0]) - 0.5;
  sensorPoint[1] = static_cast<double>(point[1]) - 0.5;
  if (NInputDimensions > 2)
    this->m_Transformer->LineSampleHeightToWorld(sensorPoint, static_cast<double>(point[2]), worldPoint);
  else
    this->m_Transformer->LineSampleToWorld(sensorPoint, worldPoint);

  OutputPointType pOut;
  pOut[0] = static_cast<TScalarType>(worldPoint[0]);
  pOut[1] = static_cast<TScalarType>(worldPoint[1]);
  if (NOutputDimensions > 2)
    pOut[2] = static_cast<TScalarType>(worldPoint[2]);
  return pOut;
}

/**
 * PrintSelf method
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SarForwardTransform<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Transformation direction: Forward" << std::endl;
}

}

#endif
