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

#ifndef otbRPCInverseTransform_hxx
#define otbRPCInverseTransform_hxx

#include "otbRPCInverseTransform.h"

namespace otb
{

template <class TScalarType>
typename RPCInverseTransform<TScalarType>::OutputPointType
RPCInverseTransform<TScalarType>::TransformPoint(const RPCInverseTransform<TScalarType>::InputPointType& point) const
{
  double x = static_cast<double>(point[0]);
  double y = static_cast<double>(point[1]);
  double z = static_cast<double>(point[2]);

  this->m_Transformer->InverseTransform(&x, &y, &z);

  OutputPointType pOut;
  pOut[0] = static_cast<TScalarType>(x);
  pOut[1] = static_cast<TScalarType>(y);
  return pOut;
}

/**
 * PrintSelf method
 */
template <class TScalarType>
void RPCInverseTransform<TScalarType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Transformation direction: Inverse" << std::endl;
}

}

#endif
