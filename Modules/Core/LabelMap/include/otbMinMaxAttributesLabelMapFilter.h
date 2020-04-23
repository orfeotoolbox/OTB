/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMinMaxAttributesLabelMapFilter_h
#define otbMinMaxAttributesLabelMapFilter_h

#include "itkLabelMapFilter.h"
#include "itkSimpleDataObjectDecorator.h"

namespace otb
{

/** \class MinMaxAttributesLabelMapFilter
 * \brief Computes the min/max of all attributes of a LabelMap
 *        where LabelObject type is compatible with otb::AttributesMapLabelObject
 *
 *
 * \ingroup OTBLabelMap
 */
template <class TInputImage>
class ITK_EXPORT MinMaxAttributesLabelMapFilter : public itk::LabelMapFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef MinMaxAttributesLabelMapFilter Self;
  typedef itk::LabelMapFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
  typedef TInputImage                                        InputImageType;
  typedef typename InputImageType::Pointer                   InputImagePointer;
  typedef typename InputImageType::ConstPointer              InputImageConstPointer;
  typedef typename InputImageType::RegionType                InputImageRegionType;
  typedef typename InputImageType::PixelType                 InputImagePixelType;
  typedef typename InputImageType::LabelObjectType           LabelObjectType;
  typedef typename InputImageType::ConstIterator             ConstIteratorType;

  typedef typename LabelObjectType::AttributesValueType     AttributesValueType;
  typedef typename LabelObjectType::AttributesMapType       AttributesMapType;
  typedef itk::SimpleDataObjectDecorator<AttributesMapType> AttributesMapObjectType;

  typedef itk::DataObject         DataObjectType;
  typedef DataObjectType::Pointer DataObjectPointerType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MinMaxAttributesLabelMapFilter, LabelMapFilter);


  /** Return the computed Minimum. */
  AttributesMapType GetMinimum() const
  {
    return this->GetMinimumOutput()->Get();
  }
  AttributesMapObjectType*       GetMinimumOutput();
  const AttributesMapObjectType* GetMinimumOutput() const;

  /** Return the computed Maximum. */
  AttributesMapType GetMaximum() const
  {
    return this->GetMaximumOutput()->Get();
  }
  AttributesMapObjectType*       GetMaximumOutput();
  const AttributesMapObjectType* GetMaximumOutput() const;

  DataObjectPointerType MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

protected:
  MinMaxAttributesLabelMapFilter();
  ~MinMaxAttributesLabelMapFilter() override{};

  void GenerateData() override;

private:
  MinMaxAttributesLabelMapFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // typedef typename InputImageType::LabelObjectContainerType   LabelObjectContainerType;
  // typedef typename LabelObjectContainerType::const_iterator   LabelObjectContainerConstIterator;


}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMinMaxAttributesLabelMapFilter.hxx"
#endif

#endif
