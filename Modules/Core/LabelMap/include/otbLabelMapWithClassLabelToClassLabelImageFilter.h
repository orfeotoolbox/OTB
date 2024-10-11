/*
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

#ifndef otbLabelMapWithClassLabelToClassLabelImageFilter_h
#define otbLabelMapWithClassLabelToClassLabelImageFilter_h

#include "itkLabelMapFilter.h"

namespace otb
{

/** \class LabelMapWithClassLabelToClassLabelImageFilter
 * \brief Converts a LabelMap<LabelObjectWithClassLabel> to an image
 *        of class labels
 *
 * \ingroup OTBLabelMap
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelMapWithClassLabelToClassLabelImageFilter : public itk::LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapWithClassLabelToClassLabelImageFilter Self;
  typedef itk::LabelMapFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TOutputImage                             OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename LabelObjectType::ConstLineIterator ConstLineIteratorType;

  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;
  typedef typename OutputImageType::IndexType    IndexType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapWithClassLabelToClassLabelImageFilter, ImageToImageFilter);

protected:
  LabelMapWithClassLabelToClassLabelImageFilter();
  ~LabelMapWithClassLabelToClassLabelImageFilter() override{};

  void BeforeThreadedGenerateData() override;

  void ThreadedProcessLabelObject(LabelObjectType* labelObject) override;

private:
  LabelMapWithClassLabelToClassLabelImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapWithClassLabelToClassLabelImageFilter.hxx"
#endif

#endif
