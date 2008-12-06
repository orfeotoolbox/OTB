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
#ifndef __otbVectorDataIOBase_txx
#define __otbVectorDataIOBase_txx

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "otbVectorDataIOBase.h"

namespace otb
{
template <class TData>
VectorDataIOBase<TData>
::VectorDataIOBase() :
  m_ByteOrder(OrderNotApplicable)
{
  Reset(false);
}

template <class TData>
void
VectorDataIOBase<TData>
::Reset(const bool)
{
  m_Initialized = false;
  m_FileName = "";
}

template <class TData>
VectorDataIOBase<TData>
::~VectorDataIOBase()
{
}

template <class TData>
std::string
VectorDataIOBase<TData>
::GetByteOrderAsString(ByteOrder t) const
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


template <class TData>
void
VectorDataIOBase<TData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "ByteOrder: " << this->GetByteOrderAsString(m_ByteOrder) << std::endl;
}

} //namespace otb

#endif
