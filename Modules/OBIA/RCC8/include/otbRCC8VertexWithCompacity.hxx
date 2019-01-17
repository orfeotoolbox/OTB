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

#ifndef otbRCC8VertexWithCompacity_hxx
#define otbRCC8VertexWithCompacity_hxx

#include "otbRCC8VertexWithCompacity.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath, class TPrecision>
RCC8VertexWithCompacity<TPath, TPrecision>
::RCC8VertexWithCompacity()
{
  m_Compacity = 0.0;
}
/**
 * Set the VertexWithCompacity attributes from the attributes vector.
 * \param attributes The vector containing the parsed attributes.
 */
template <class TPath, class TPrecision>
void
RCC8VertexWithCompacity<TPath, TPrecision>
::SetAttributesMap(AttributesMapType attributes)
{
  this->Superclass::SetAttributesMap(attributes);
  m_Compacity = static_cast<TPrecision>(atof(attributes["Compacity"].c_str()));
}
/**
 * Get an attributes vector representing the VertexBase attributes.
 * \return The attributes vector
 */
template <class TPath, class TPrecision>
typename RCC8VertexWithCompacity<TPath, TPrecision>::AttributesMapType
RCC8VertexWithCompacity<TPath, TPrecision>
::GetAttributesMap(void)
{
  std::stringstream oss;
  AttributesMapType results;
  results = this->Superclass::GetAttributesMap();
  oss << m_Compacity;
  results["Compacity"] = oss.str();
  oss.str("");
  return results;
}
/**
 * PrintSelf method
 */
template <class TPath, class TPrecision>
void
RCC8VertexWithCompacity<TPath, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Compacity: " << m_Compacity << std::endl;
}
} // end namespace otb
#endif
