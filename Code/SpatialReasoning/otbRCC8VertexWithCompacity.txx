/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbRCC8VertexWithCompacity_txx
#define __otbRCC8VertexWithCompacity_txx

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
