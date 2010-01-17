/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelObject.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelObjectUtils_h
#define __itkLabelObjectUtils_h

#include <deque>
#include <itkLightObject.h>
#include "itkLabelMap.h"
#include "itkLabelObjectLine.h"

namespace itk
{


namespace Functor {

template< class TLabelObject >
class ITK_EXPORT LabelLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::LabelType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetLabel();
    }
};

template< class TLabelObject >
class ITK_EXPORT NumberOfLinesLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef int AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetNumberOfLines();
    }
};

template< class TLabelObject, class TAttributeAccessor >
class LabelObjectComparator
{
public:
  typedef TLabelObject LabelObjectType;
  typedef TAttributeAccessor AttributeAccessorType;
  bool operator()( const LabelObjectType * a, const LabelObjectType * b )
    {
    return accessor( a ) > accessor( b );
    }
private:
  AttributeAccessorType accessor;
};

template< class TLabelObject, class TAttributeAccessor >
class LabelObjectReverseComparator
{
public:
  typedef TLabelObject LabelObjectType;
  typedef TAttributeAccessor AttributeAccessorType;
  bool operator()( const LabelObjectType * a, const LabelObjectType * b )
    {
    return accessor( a ) < accessor( b );
    }
private:
  AttributeAccessorType accessor;
};

}

} // end namespace itk

#endif
