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

#ifndef otbSarTransformBase_hxx
#define otbSarTransformBase_hxx

#include "otbSarTransformBase.h"
#include "otbLogger.h"
#define UNUSED(x) [&x]{}()

namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool SarTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::SetMetadata(const ImageMetadata& imd)
{
  if (!imd.Has(MDGeom::SAR))
    return false;
  const boost::any any_Sar = imd[MDGeom::SAR];
  if (any_Sar.empty())
    return false;
  try
  {
    this->m_SarParam = std::make_unique<SARParam>(boost::any_cast<SARParam>(imd[MDGeom::SAR]));
  }
  catch (const boost::bad_any_cast&)
  {
    return false;
  }

  this->m_Transformer   = std::make_unique<SarSensorModel>(imd);
  return true;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
bool SarTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::IsValidSensorModel() const
{
  return m_Transformer != nullptr;
}

/**
 * PrintSelf method
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SarTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Sar Model" << std::endl;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SarTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::OptimizeParameters(ImageMetadata& /*imd*/, const TiePointsType& /*tiepoints*/, double& /*rmsError*/)
{
    // A Standard definition of this function is available for RPC sensor model in RPCTransformBase class
    otbLogMacro(Warning, << "Optimize parameters function not implemented for SAR sensor model. The sensor model parameters will NOT be modified");
}

} // namespace otb

#endif
