/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageIOBase.cxx,v $
  Language:  C++
  Date:      $Date: 2007/09/15 18:29:56 $
  Version:   $Revision: 1.70 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "otbVectorDataIOBase.h"

namespace otb
{

VectorDataIOBase::VectorDataIOBase() :
  m_ByteOrder(OrderNotApplicable)
{
  Reset(false);
}

  
void VectorDataIOBase::Reset(const bool)
{
  m_Initialized = false;
  m_FileName = "";
}

VectorDataIOBase::~VectorDataIOBase()
{
}


std::string VectorDataIOBase::GetByteOrderAsString(ByteOrder t) const
{
  std::string s;
  switch(t)
    {
    case BigEndian:
      return s = "BigEndian";
    case LittleEndian:
      return s = "LittleEndian";
    case OrderNotApplicable:
    default:
      return s = "OrderNotApplicable";
    }
  return s="OrderNotApplicable";
}



void VectorDataIOBase::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "ByteOrder: " << this->GetByteOrderAsString(m_ByteOrder) << std::endl;
}

} //namespace otb
