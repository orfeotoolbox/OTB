/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8VertexWithRegionCenter_hxx
#define otbRCC8VertexWithRegionCenter_hxx

#include "otbRCC8VertexWithRegionCenter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TLabel, class TPrecision>
RCC8VertexWithRegionCenter<TLabel, TPrecision>
::RCC8VertexWithRegionCenter()
{
  m_RegionCenterXCoordinate = 0.0;
  m_RegionCenterYCoordinate = 0.0;
}
/**
 * Set the VertexWithRegionCenter attributes from the attributes vector.
 * \param attributes The vector containing the parsed attributes.
 */
template <class TLabel, class TPrecision>
void
RCC8VertexWithRegionCenter<TLabel, TPrecision>
::SetAttributesMap(AttributesMapType attributes)
{
  this->Superclass::SetAttributesMap(attributes);
  m_RegionCenterXCoordinate = static_cast<TPrecision>(atof(attributes["RegionCenterXCoordinate"].c_str()));
  m_RegionCenterYCoordinate = static_cast<TPrecision>(atof(attributes["RegionCenterYCoordinate"].c_str()));
}
/**
 * Get an attributes vector representing the VertexBase attributes.
 * \return The attributes vector
 */
template <class TLabel, class TPrecision>
typename RCC8VertexWithRegionCenter<TLabel, TPrecision>::AttributesMapType
RCC8VertexWithRegionCenter<TLabel, TPrecision>
::GetAttributesMap(void)
{
  std::stringstream oss;
  AttributesMapType results;
  results = this->Superclass::GetAttributesMap();
  oss << m_RegionCenterXCoordinate;
  results["RegionCenterXCoordinate"] = oss.str();
  oss.str("");
  oss << m_RegionCenterYCoordinate;
  results["RegionCenterYCoordinate"] = oss.str();
  oss.str("");
  return results;
}
/**
 * PrintSelf method
 */
template <class TLabel, class TPrecision>
void
RCC8VertexWithRegionCenter<TLabel, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "RegionCenterXCoordinate: " << m_RegionCenterXCoordinate << std::endl;
  os << indent << "RegionCenterXCoordinate: " << m_RegionCenterYCoordinate << std::endl;
}
} // end namespace otb
#endif
