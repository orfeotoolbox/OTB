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

#ifndef otbSarInverseTransform_hxx
#define otbSarInverseTransform_hxx

#include "otbSarInverseTransform.h"
#include "otbDEMHandler.h"

namespace otb
{
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
SarInverseTransform<TScalarType, NInputDimensions, NOutputDimensions>::SarInverseTransform() : Superclass(TransformDirection::INVERSE)
{}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename SarInverseTransform<TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
SarInverseTransform<TScalarType, NInputDimensions, NOutputDimensions>::TransformPoint(const InputPointType& point) const
{
  SarSensorModel::Point2DType sensorPoint;
  SarSensorModel::Point3DType worldPoint;
  worldPoint[0] = static_cast<double>(point[0]);
  worldPoint[1] = static_cast<double>(point[1]);
  if (NInputDimensions > 2)
    worldPoint[2] = static_cast<double>(point[2]);
  else
    worldPoint[2] = otb::DEMHandler::GetInstance().GetHeightAboveEllipsoid(point[0], point[1]);

  this->m_Transformer->WorldToLineSample(worldPoint, sensorPoint);

  OutputPointType pOut;

  // from centered to upper left corner pixel convention
  pOut[0] = static_cast<TScalarType>(sensorPoint[0]) + 0.5;
  pOut[1] = static_cast<TScalarType>(sensorPoint[1]) + 0.5;

  if (NOutputDimensions > 2)
    pOut[2] = static_cast<TScalarType>(worldPoint[2]);

  return pOut;
}

/**
 * PrintSelf method
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SarInverseTransform<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Transformation direction: Inverse" << std::endl;
}

}

#endif
