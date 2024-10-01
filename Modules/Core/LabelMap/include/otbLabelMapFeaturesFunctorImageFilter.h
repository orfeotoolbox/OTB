/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbLabelMapFeaturesFunctorImageFilter_h
#define otbLabelMapFeaturesFunctorImageFilter_h

#include "itkInPlaceLabelMapFilter.h"

namespace otb
{

/** \class LabelMapFeaturesFunctorImageFilter
 *  \brief This class applies a functor to compute new features
 *
 *  The functor is applied on each LabelObject, enriching the
 *  available features.
 *
 *  The LabelObject type must be an AttributeMapLabelObject.
 *
 *  This filter can not be instantiated on its own, since its purpose
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
 *
 * \ingroup OTBLabelMap
 */
template <class TImage, class TFunctor>
class ITK_EXPORT LabelMapFeaturesFunctorImageFilter : public itk::InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapFeaturesFunctorImageFilter Self;
  typedef itk::InPlaceLabelMapFilter<TImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage                              ImageType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  typedef TFunctor                            FunctorType;

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
  LabelMapFeaturesFunctorImageFilter() : m_Functor()
  {
  }

  /** Destructor */
  ~LabelMapFeaturesFunctorImageFilter() override
  {
  }

  /** Threaded generate data */
  void ThreadedProcessLabelObject(LabelObjectType* labelObject) override
  {
    // Call the functor
    m_Functor(labelObject);
  }

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    // Call superclass implementation
    Superclass::PrintSelf(os, indent);
  }

private:
  LabelMapFeaturesFunctorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The functor */
  FunctorType m_Functor;

}; // end of class

} // end namespace otb

#endif
