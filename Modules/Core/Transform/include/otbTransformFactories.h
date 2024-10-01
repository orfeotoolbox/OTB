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

#ifndef otbTransformFactory_h
#define otbTransformFactory_h

#include "otbCast.h"
#include "otbImageMetadata.h"
#include "otbSensorTransformBase.h"
#include "otbTransform.h"

#include "otbRPCForwardTransform.h"
#include "otbRPCInverseTransform.h"
#include "otbSarForwardTransform.h"
#include "otbSarInverseTransform.h"
#include "otbSpot5ForwardTransform.h"
#include "otbSpot5InverseTransform.h"

namespace otb {

/** \namespace TransformFactories
 * \brief Contains the functions (factories) used to instantiate the transformation classes.
 *
 * Each function is in charge of instantiating a specific transformation classes. It is
 * templated with the scalar type used in the Sensor Model, the dimension of the
 * inpute space, and the dimension of the output space. It takes as parameter a
 * reference to the ImageMetadata object used to instantiate the Sensor Model, and the
 * TransformDirection. It returns an instance of otb::SensorTransformBase.
 *
 * \ingroup OTBTransform
 */
namespace TransformFactories {

/**
 * Factory for the forward transformer based on the RPC sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
RPCForwardTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::RPC))
  {
    auto transform = RPCForwardTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

/**
 * Factory for the inverse transformer based on the RPC sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
RPCInverseTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::RPC))
  {
    auto transform = RPCInverseTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

/**
 * Factory for the forward transformer based on the SAR sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
SARForwardTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::SAR))
  {
    auto transform = SarForwardTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

/**
 * Factory for the inverse transformer based on the SAR sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
SARInverseTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::SAR))
  {
    auto transform = SarInverseTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

/**
 * Factory for the forward transformer based on the Spot5 sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
Spot5ForwardTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::Spot5Geometry))
  {
    auto transform = Spot5ForwardTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

/**
 * Factory for the inverse transformer based on the Spot5 sensor model
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>::Pointer
Spot5InverseTransformFactory(const ImageMetadata &imd, TransformDirection direction)
{
  if(imd.Has(MDGeom::Spot5Geometry))
  {
    auto transform = Spot5InverseTransform<TScalarType, NInputDimensions,NOutputDimensions>::New();
    if(transform->getDirection() != direction)
      return nullptr;
    transform->SetMetadata(imd);
    return DynamicCast<typename otb::SensorTransformBase<double, NInputDimensions, NOutputDimensions>>(transform);
  }
  return nullptr;
}

}
}

#endif
