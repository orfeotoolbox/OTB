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

#ifndef otbSensorModelBase_hxx
#define otbSensorModelBase_hxx

#include "otbSensorModelBase.h"


namespace otb
{

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>::SensorModelBase() : Superclass(0)
{
  m_Model = SensorModelAdapter::New();
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>::~SensorModelBase()
{
}

/// Get the Geometry Keyword list
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
ImageKeywordlist SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>::GetImageGeometryKeywordlist(void) const
{
  return m_ImageKeywordlist;
}

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>::SetImageGeometry(const ImageKeywordlist& image_kwl)
{
  m_ImageKeywordlist = image_kwl;
  m_Model->CreateProjection(m_ImageKeywordlist);
}

/**
 * PrintSelf method
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
void SensorModelBase<TScalarType, NInputDimensions, NOutputDimensions>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Model: " << m_Model << std::endl;
  os << indent << "Keywordlist: " << m_ImageKeywordlist << std::endl;
}

} // namespace otb

#endif
