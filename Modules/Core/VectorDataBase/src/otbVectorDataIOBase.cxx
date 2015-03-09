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

#include "otbVectorDataIOBase.h"

namespace otb
{
VectorDataIOBase
::VectorDataIOBase() :
  m_ByteOrder(OrderNotApplicable)
{
  this->Reset(false);
}

void
VectorDataIOBase
::Reset(const bool)
{
  m_Initialized = false;
  m_FileName = "";
}


VectorDataIOBase
::~VectorDataIOBase()
{}

std::string
VectorDataIOBase
::GetByteOrderAsString(ByteOrder t) const
{
  std::string s;
  switch (t)
    {
    case BigEndian:
      return s = "BigEndian";
    case LittleEndian:
      return s = "LittleEndian";
    case OrderNotApplicable:
    default:
      return s = "OrderNotApplicable";
    }
}

void
VectorDataIOBase
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "ByteOrder: " << this->GetByteOrderAsString(m_ByteOrder) << std::endl;
}

} //namespace otb
