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
#ifndef otbSensorTransformFactory_hxx
#define otbSensorTransformFactory_hxx

#include "otbImageMetadata.h"
#include "otbSensorTransformFactory.h"
#include "otbTransform.h"
#include "otbTransformFactories.h"
#include <functional>

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::Pointer
SensorTransformFactory::CreateTransform(const ImageMetadata &imd, TransformDirection direction) const
{
  // Instantiate the factories
  std::vector<
    std::function<
      typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer (const ImageMetadata &,
                                                                                               TransformDirection direction)
    >
  > factories;
  factories.push_back(TransformFactories::RPCForwardTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);
  factories.push_back(TransformFactories::RPCInverseTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);
  factories.push_back(TransformFactories::SARForwardTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);
  factories.push_back(TransformFactories::SARInverseTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);
  factories.push_back(TransformFactories::Spot5ForwardTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);
  factories.push_back(TransformFactories::Spot5InverseTransformFactory<TScalarType, NInputDimensions, NOutputDimensions>);

  typename otb::SensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::Pointer transformPointer;
  for (auto& transformFactory : factories)
  {
    transformPointer = transformFactory(imd, direction);
    if (transformPointer)
    {
      if (transformPointer->IsValidSensorModel() && transformPointer->getDirection() == direction)
      {
        return transformPointer;
      }
    }
  }
  // if no object has been found, we have to warn the user
  otbLogMacro(Warning, << "The SensorTransform factory could not find a compatible Sensor Transform");
  return nullptr;
}

} // end namespace otb

#endif
