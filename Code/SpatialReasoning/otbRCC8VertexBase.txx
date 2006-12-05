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
#ifndef _otbRCC8VertexBase_txx
#define _otbRCC8VertexBase_txx

#include "otbRCC8VertexBase.h"

namespace otb
{
/**
 * Constructor
 */
template <class TLabel>
RCC8VertexBase<TLabel>
::RCC8VertexBase()
{
  m_SegmentationImageIndex=0;
  m_ObjectLabelInImage=0;
}
/**
 * Set the VertexBase attributes from the attributes vector.
 * \param attributes The vector containing the parsed attributes.
 */
template <class TLabel>
void
RCC8VertexBase<TLabel>
::SetAttributesVector(AttributesVectorType attributes)
{
  m_SegmentationImageIndex=static_cast<unsigned int>(atoi(attributes[1].c_str()));
  m_ObjectLabelInImage=static_cast<LabelType>(atof(attributes[3].c_str()));
}
/**
 * Get an attributes vector representing the VertexBase attributes.
 * \return The attributes vector
 */
template <class TLabel>
typename RCC8VertexBase<TLabel>::AttributesVectorType
RCC8VertexBase<TLabel>
::GetAttributesVector(void)
{  
  std::stringstream oss;
  AttributesVectorType results;
  results.push_back("SegmentationImageIndex");
  oss<<m_SegmentationImageIndex;
  results.push_back(oss.str());
  oss.str("");
  results.push_back("ObjectLabelInImage");
  oss<<static_cast<unsigned int>(m_ObjectLabelInImage);
  results.push_back(oss.str());
  oss.str("");
  return results;
}
/**
 * PrintSelf method
 */
template <class TLabel>
void
RCC8VertexBase<TLabel>
::PrintSelf( std::ostream& os,itk::Indent indent ) const
  {
    Superclass::PrintSelf(os,indent);
    os<<indent<<"SegmentationImageIndex: "<<m_SegmentationImageIndex<<std::endl;
    os<<indent<<"ObjectLabelInImage: "<<m_ObjectLabelInImage<<std::endl;
  }
} // end namespace otb
#endif
