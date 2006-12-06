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
#ifndef _otbRCC8VertexWithCompacity_txx
#define _otbRCC8VertexWithCompacity_txx

#include "otbRCC8VertexWithCompacity.h"

namespace otb
{
/**
 * Constructor
 */
template <class TLabel,class TPrecision>
RCC8VertexWithCompacity<TLabel,TPrecision>
::RCC8VertexWithCompacity()
{
  m_Compacity=0.0;
}
/**
 * Set the VertexWithCompacity attributes from the attributes vector.
 * \param attributes The vector containing the parsed attributes.
 */
template <class TLabel,class TPrecision>
void
RCC8VertexWithCompacity<TLabel,TPrecision>
::SetAttributesMap(AttributesMapType attributes)
{
  this->Superclass::SetAttributesMap(attributes);
  m_Compacity=static_cast<TPrecision>(atof(attributes["Compacity"].c_str()));
}
/**
 * Get an attributes vector representing the VertexBase attributes.
 * \return The attributes vector
 */
template <class TLabel,class TPrecision>
typename RCC8VertexWithCompacity<TLabel,TPrecision>::AttributesMapType
RCC8VertexWithCompacity<TLabel,TPrecision>
::GetAttributesMap(void)
{  
  std::stringstream oss;
  AttributesMapType results;
  results=this->Superclass::GetAttributesMap();
  oss<<m_Compacity;
  results["Compacity"]=oss.str();
  oss.str("");
  return results;
}
/**
 * PrintSelf method
 */
template <class TLabel,class TPrecision>
void
RCC8VertexWithCompacity<TLabel,TPrecision>
::PrintSelf( std::ostream& os,itk::Indent indent ) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"Compacity: "<<m_Compacity<<std::endl;
  }
} // end namespace otb
#endif
