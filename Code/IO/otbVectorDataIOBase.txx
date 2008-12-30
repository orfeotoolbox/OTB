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
  m_Spacing.Fill(1);
  m_Origin.Fill(0);
}


//----------------------------------------------------------------------------
template <class TData>
    void
        VectorDataIOBase<TData>
  ::SetSpacing(const SpacingType & spacing )
{
  itkDebugMacro("setting Spacing to " << spacing);
  if( this->m_Spacing != spacing )
  {
    this->m_Spacing = spacing;
    this->Modified();
  }
}


//----------------------------------------------------------------------------
template <class TData>
    void
        VectorDataIOBase<TData>
  ::SetSpacing(const double spacing[VDimension] )
{
  SpacingType s(spacing);
  this->SetSpacing(s);
}


//----------------------------------------------------------------------------
template <class TData>
    void
        VectorDataIOBase<TData>
  ::SetSpacing(const float spacing[VDimension] )
{
  itk::Vector<float, VDimension> sf(spacing);
  SpacingType s;
  s.CastFrom( sf );
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TData>
    void
        VectorDataIOBase<TData>
  ::SetOrigin(const double origin[VDimension] )
{
  PointType p(origin);
  this->SetOrigin( p );
}


//----------------------------------------------------------------------------
template <class TData>
    void
        VectorDataIOBase<TData>
  ::SetOrigin(const float origin[VDimension] )
{
  itk::Point<float, VDimension> of(origin);
  PointType p;
  p.CastFrom( of );
  this->SetOrigin( p );
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
