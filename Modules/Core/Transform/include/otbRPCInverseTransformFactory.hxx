/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbRPCInverseTransformFactory_hxx
#define otbRPCInverseTransformFactory_hxx

#include "otbRPCInverseTransformFactory.h"
#include "otbRPCInverseTransform.h"
#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::RPCInverseTransformFactory()
{
  this->RegisterOverride("otbRPCTransformBase", "otbRPCInverseTransform", "RPC Inverse Transform", 1,
                           itk::CreateObjectFunction<RPCInverseTransform<TScalarType,NInputDimensions,NOutputDimensions>>::New());
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetDescription() const
{
  return "RPC Inverse Transform factory";
}

}

#endif
