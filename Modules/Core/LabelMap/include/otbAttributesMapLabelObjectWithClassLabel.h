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
#ifndef __otbAttributesMapLabelObjectWithClassLabel_h
#define __otbAttributesMapLabelObjectWithClassLabel_h

#include "otbAttributesMapLabelObject.h"

namespace otb
{

/** \class AttributesMapLabelObjectWithClassLabel
 *  \brief An AttributesMapLabelObject with an optional class label.
 *
 *  The label type is defined by the template parameter TClassLabel and
 *  accessible using the ClassLabelType typedef.
 *
 * The HasClassLabel() method returns true if the LabelObject has a
 * class label and false otherwise.
 *
 * The SetClassLabel() method set the class label and set the internal flag
 * m_HasClassLabel to true.
 *
 * The GetClassLabel() method returns the class label or an exception if m_HasClassLabel
 * is set to false.
 *
 * The RemoveClassLabel() method set m_HasClassLabel to false and the
 * class label value to a default value.
 *
 *\sa LabelObject, ShapeLabelObject, StatisticsLabelObject
 *
 * \ingroup DataRepresentation
 */
template < class TLabel, unsigned int VImageDimension, class TAttributesValue, class TClassLabel >
class ITK_EXPORT AttributesMapLabelObjectWithClassLabel : public AttributesMapLabelObject<TLabel, VImageDimension, TAttributesValue>
{
public:
  /** Standard class typedefs */
  typedef AttributesMapLabelObjectWithClassLabel      Self;
  typedef AttributesMapLabelObject<TLabel,
      VImageDimension, TAttributesValue>  Superclass;
  typedef itk::LabelObject<TLabel, VImageDimension>    LabelObjectType;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;
  typedef itk::WeakPointer <const Self>               ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AttributesMapLabelObjectWithClassLabel, AttributesMapLabelObject);

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Class label typedef */
  typedef TClassLabel                                  ClassLabelType;

  /** Set the class label */
  void SetClassLabel(const ClassLabelType& label)
  {
    m_ClassLabel = label;
    m_HasClassLabel = true;
  }

  /** Get the class label. Throws an exception if no class label is
   *  available */
  const ClassLabelType & GetClassLabel() const
  {
    if(!m_HasClassLabel)
      {
      itkExceptionMacro(<<"LabelObject has no class label!");
      }
    return m_ClassLabel;
  }

  /** \return true if a class label is available */
  bool HasClassLabel() const
  {
    return m_HasClassLabel;
  }

  /** Invalidate the class label if any */
  void RemoveClassLabel()
  {
    m_ClassLabel = itk::NumericTraits<ClassLabelType>::Zero;
    m_HasClassLabel = false;
  }

  virtual void CopyAttributesFrom( const LabelObjectType * lo )
    {
    Superclass::CopyAttributesFrom( lo );

    // copy the data of the current type if possible
    const Self * src = dynamic_cast<const Self *>( lo );
    if( src == NULL )
      {
      return;
      }

    m_ClassLabel = src->m_ClassLabel;
    m_HasClassLabel = src->m_HasClassLabel;
    }

protected:
  /** Constructor */
  AttributesMapLabelObjectWithClassLabel() : m_ClassLabel(itk::NumericTraits<ClassLabelType>::Zero), m_HasClassLabel(false)
    {}
  /** Destructor */
  virtual ~AttributesMapLabelObjectWithClassLabel() {}

  /** The printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf( os, indent );
      if(m_HasClassLabel)
  {
  os<<indent<<"Class Label: "<<m_ClassLabel<<std::endl;
  }
      else
  {
  os<<indent<<"No class label available."<<std::endl;
  }
    }

private:
  AttributesMapLabelObjectWithClassLabel(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The class label */
  ClassLabelType m_ClassLabel;

  /** Does the LabelObject have a class label ? */
  bool           m_HasClassLabel;

};

} // end namespace otb
#endif
