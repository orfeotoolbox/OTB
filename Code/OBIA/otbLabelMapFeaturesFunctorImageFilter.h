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
#ifndef __otbLabelMapFeaturesFunctorImageFilter_h
#define __otbLabelMapFeaturesFunctorImageFilter_h

#include "itkInPlaceLabelMapFilter.h"

namespace otb {

/** \class LabelMapFeaturesFunctorImageFilter
 *  \brief This class applies a functor to compute new features
 *
 *  The functor is applied on each LabelObject, enriching the
 *  available features.
 *
 *  The LabelObject type must be an AttributeMapLabelObject.
 *
 *  This filter can not be instanciated on its own, since its purpose
 *  is to provide a base class for all LabelMap attributes enriching filters
 *
 * \sa otb::AttributeMapLabelObject
 * \sa otb::StatisticsAttributesLabelMapFilter
 * \sa otb::ShapeAttributesLabelMapFilter
 * \sa otb::RadiometricAttributesLabelMapFilter
 * \sa otb::BandsStatisticsAttributesLabelMapFilter
 * \sa itk::InPlaceLabelMapFilter
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TFunctor>
class ITK_EXPORT LabelMapFeaturesFunctorImageFilter :
  public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapFeaturesFunctorImageFilter Self;
  typedef itk::InPlaceLabelMapFilter<TImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage                               ImageType;
  typedef typename  ImageType::LabelObjectType LabelObjectType;
  typedef TFunctor                             FunctorType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Runtime information support. */
  itkTypeMacro(LabelMapFeaturesFunctorImageFilter, InPlaceLabelMapFilter);

  /** Set the functor */
  void SetFunctor(FunctorType& functor)
  {
    if (m_Functor != functor)
      {
      m_Functor = functor;
      this->Modified();
      }
  }

  /** Get the functor (const version) */
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  }

  /** Get a reference to the functor (non const version) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }

protected:
  /** Constructor */
  LabelMapFeaturesFunctorImageFilter() : m_Functor() {}

  /** Destructor */
  ~LabelMapFeaturesFunctorImageFilter() {}

  /** Threaded generate data */
  virtual void ThreadedProcessLabelObject(LabelObjectType * labelObject)
  {
    // Call the functor
    m_Functor(labelObject);
  }

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    // Call superclass implementation
    Superclass::PrintSelf(os, indent);
  }

private:
  LabelMapFeaturesFunctorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The functor */
  FunctorType m_Functor;

}; // end of class

} // end namespace otb

#endif
