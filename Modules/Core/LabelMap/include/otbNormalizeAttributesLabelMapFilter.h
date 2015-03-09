/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbNormalizeAttributesLabelMapFilter_h
#define __otbNormalizeAttributesLabelMapFilter_h

#include "otbLabelMapFeaturesFunctorImageFilter.h"
#include <vector>

namespace otb
{

namespace Functor
{
/** \class NormalizeAttributesLabelObjectFunctor
*   \brief Functor to normalize all attributes of a LabelMap
*
*   Given the minimum and maximum of all attributes (which can be computed with
*   MinMaxAttributesLabelMapFilter), this filter rescale the attributes such that
*   all attributes are in [0, 1] interval
*
*   \sa MinMaxAttributesLabelMapFilter
 *
 * \ingroup OTBLabelMap
*/
template <class TLabelObject>
class NormalizeAttributesLabelObjectFunctor
{
public:
  /** Self typedef */
  typedef NormalizeAttributesLabelObjectFunctor Self;

  /** LabelObject typedef */
  typedef TLabelObject                                  LabelObjectType;
  typedef typename LabelObjectType::AttributesMapType   AttributesMapType;
  typedef typename LabelObjectType::AttributesValueType AttributesValueType;

  /** Constructor */
  NormalizeAttributesLabelObjectFunctor();

  /** Destructor */
  virtual ~NormalizeAttributesLabelObjectFunctor() {}

  /** The comparators */
  bool operator !=(const Self& self);
  bool operator ==(const Self& self);

  /** This is the functor implementation
   *  Calling the functor on a label object
   *  will update its shape attributes */
  inline void operator ()(LabelObjectType * lo) const;

  void SetMinAttributesValues(const AttributesMapType& minValues)
  {
    m_Min = minValues;
  }

  void SetMaxAttributesValues(const AttributesMapType& maxValues)
  {
    m_Max = maxValues;
  }

private:
  AttributesMapType m_Min;
  AttributesMapType m_Max;
};

}

/** \class NormalizeAttributesLabelMapFilter
 * \brief This class works with AttributesMapLabelObject, and normalizes all attributes
 *        wrt a map of minimal and maximal attribute value, such that the resulting
 *        attributes are in the [0, 1] interval
 *
 * \sa MinMaxAttributesLabelMapFilter
 *
 * \ingroup OTBLabelMap
 */
template<class TImage>
class ITK_EXPORT NormalizeAttributesLabelMapFilter
  : public otb::LabelMapFeaturesFunctorImageFilter
  <TImage,
      typename Functor::NormalizeAttributesLabelObjectFunctor
      <typename TImage::LabelObjectType> >
{
public:
  /** Template parameters typedefs */
  typedef TImage                                        ImageType;
  typedef typename ImageType::LabelObjectType           LabelObjectType;
  typedef typename LabelObjectType::AttributesMapType   AttributesMapType;

  typedef Functor::NormalizeAttributesLabelObjectFunctor
  <LabelObjectType>                           FunctorType;

  /** Standard class typedefs. */
  typedef NormalizeAttributesLabelMapFilter Self;
  typedef LabelMapFeaturesFunctorImageFilter
  <ImageType, FunctorType>                  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(NormalizeAttributesLabelMapFilter, LabelMapFeaturesFunctorImageFilter);

  void SetMinAttributesValues(const AttributesMapType& minValues)
  {
    this->GetFunctor().SetMinAttributesValues(minValues);
    this->Modified();
  }

  void SetMaxAttributesValues(const AttributesMapType& maxValues)
  {
    this->GetFunctor().SetMaxAttributesValues(maxValues);
    this->Modified();
  }

protected:
  /** Constructor */
  NormalizeAttributesLabelMapFilter(){}

  /** Destructor */
  virtual ~NormalizeAttributesLabelMapFilter(){}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  NormalizeAttributesLabelMapFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNormalizeAttributesLabelMapFilter.txx"
#endif

#endif
