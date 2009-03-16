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
#ifndef __itkAttributeLabelObject_h
#define __itkAttributeLabelObject_h

#include "itkLabelObject.h"

namespace itk
{


namespace Functor {

template< class TLabelObject >
class ITK_EXPORT AttributeLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::AttributeValueType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetAttribute();
    }

  inline void operator()( LabelObjectType * labelObject, AttributeValueType value )
    {
    labelObject->SetAttribute( value );
    }
};

}


/** \class AttributeLabelObject
 *  \brief A LabelObject with a generic attribute
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 *\sa LabelObject, ShapeLabelObject, StatisticsLabelObject
 *
 * \ingroup DataRepresentation 
 */
template < class TLabel, unsigned int VImageDimension, class TAttributeValue >
class ITK_EXPORT AttributeLabelObject : public LabelObject< TLabel, VImageDimension >
{
public:
  /** Standard class typedefs */
  typedef AttributeLabelObject         Self;
  typedef LabelObject< TLabel, VImageDimension > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AttributeLabelObject, LabelObject);

  typedef LabelMap< Self > LabelMapType;

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef typename Superclass::IndexType IndexType;

  typedef TLabel LabelType;

  typedef typename Superclass::LineType LineType;

  typedef typename Superclass::LengthType LengthType;

  typedef typename Superclass::LineContainerType LineContainerType;

  typedef TAttributeValue AttributeValueType;

  void SetAttribute( const AttributeValueType & v )
    {
    m_Attribute = v;
    }

  const AttributeValueType & GetAttribute() const
    {
    return m_Attribute;
    }

  AttributeValueType GetAttribute()
    {
    return m_Attribute;
    }


  virtual void CopyDataFrom( const Self * src )
    {
    Superclass::CopyDataFrom( src );
    m_Attribute = src->m_Attribute;
    }

protected:
  AttributeLabelObject()
    {
    // how to initialize the attribute ?
    }
  

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );

    os << indent << "Attribute: " << m_Attribute << std::endl;
    }

private:
  AttributeLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  AttributeValueType m_Attribute;
  

};

} // end namespace itk

#endif
