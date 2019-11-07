/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbAttributesMapOpeningLabelMapFilter_h
#define otbAttributesMapOpeningLabelMapFilter_h

#include "otbAttributesMapLabelObject.h"
#include "itkAttributeOpeningLabelMapFilter.h"

namespace otb
{

/** \class AttributesMapOpeningLabelMapFilter
 * \brief Attributes opening for AttributesMapLabelObject
 *
 * This class extends the itk::AttributeOpeningLabelMapFilter to
 * handle AttributesMapLabelObject. Thus it allows performing openings
 * for any kind of attributes in the attributes map.
 *
 * \sa AttributeLabelObject, AttributeOpeningLabelMapFilter
 *
 * \ingroup ImageEnhancement MathematicalMorphologyImageFilters
 *
 * \ingroup OTBLabelMap
 */
template <class TImage>
class ITK_EXPORT AttributesMapOpeningLabelMapFilter
    : public itk::AttributeOpeningLabelMapFilter<TImage, typename otb::Functor::AttributesMapLabelObjectAccessor<typename TImage::LabelObjectType>>
{
public:
  /** Standard class typedefs. */
  typedef AttributesMapOpeningLabelMapFilter Self;
  typedef itk::AttributeOpeningLabelMapFilter<TImage, Functor::AttributesMapLabelObjectAccessor<typename TImage::LabelObjectType>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::AttributeAccessorType AttributeAccessorType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AttributesMapOpeningLabelMapFilter, itk::AttributeOpeningLabelMapFilter);

  /** Set the name of the attribute to perform opening on */
  void SetAttributeName(const char* name);

  /** Get the name of the attribute to perform opening on */
  const char* GetAttributeName();

  void SetAccessor(const AttributeAccessorType& accessor);

  AttributeAccessorType& GetAccessor();

  void GenerateData() override;


protected:
  /** Constructor */
  AttributesMapOpeningLabelMapFilter();
  /** Destructor */
  ~AttributesMapOpeningLabelMapFilter() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  AttributesMapOpeningLabelMapFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  AttributeAccessorType m_Accessor;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAttributesMapOpeningLabelMapFilter.hxx"
#endif

#endif // otbAttributesMapOpeningLabelMapFilter_h
