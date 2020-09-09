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
bool RPCTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::SetMetadataModel(boost::any imdModel)
{
  try
  {
    Projection::RPCParam newParam = boost::any_cast<Projection::RPCParam>(imdModel);
    this->m_RPCParam.reset(&newParam);
  }
  catch (boost::bad_any_cast)
  {
    return false;
  }
  GDALRPCTransformer newTrans = GDALRPCTransformer(
          m_RPCParam->LineOffset, m_RPCParam->SampleOffset, m_RPCParam->LatOffset, m_RPCParam->LonOffset, m_RPCParam->HeightOffset,
          m_RPCParam->LineScale, m_RPCParam->SampleScale, m_RPCParam->LatScale, m_RPCParam->LonScale, m_RPCParam->HeightScale,
          m_RPCParam->LineNum, m_RPCParam->LineDen, m_RPCParam->SampleNum, m_RPCParam->SampleDen);
  this->m_Transformer.reset(&newTrans);
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
  os << indent << "RPC Model: " << this->m_RPCParam.get() << std::endl;
}

}

#endif