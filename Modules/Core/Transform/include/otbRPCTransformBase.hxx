/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRPCTransformBase_hxx
#define otbRPCTransformBase_hxx

#include "otbRPCTransformBase.h"

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::SetMetadata(const ImageMetadata& imd)
{
  if (!imd.Has(MDGeom::RPC))
    return false;
  const boost::any any_rpc = imd[MDGeom::RPC];
  if (any_rpc.empty())
    return false;
  try
  {
    this->m_RPCParam = std::make_unique<Projection::RPCParam>(boost::any_cast<Projection::RPCParam>(imd[MDGeom::RPC]));
  }
  catch (const boost::bad_any_cast&)
  {
    return false;
  }

  constexpr bool useDEM = NInputDimensions == 2 ? true : false;
  this->m_Transformer   = std::make_unique<GDALRPCTransformer>(*m_RPCParam, useDEM);
  return true;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::IsValidSensorModel() const
{
  return m_Transformer != nullptr;
}

/**
 * PrintSelf method
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "RPC Model: " << this->m_RPCParam.get()->ToJSON() << std::endl;
}

} // namespace otb

#endif
