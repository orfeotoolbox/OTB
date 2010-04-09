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
#ifndef __otbRCC8VertexWithRegionCenter_txx
#define __otbRCC8VertexWithRegionCenter_txx

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
