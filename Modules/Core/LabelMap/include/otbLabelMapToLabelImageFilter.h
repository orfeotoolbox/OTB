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

#ifndef otbLabelMapToLabelImageFilter_h
#define otbLabelMapToLabelImageFilter_h

#include "itkLabelMapToLabelImageFilter.h"

namespace otb
{

/** \class LabelMapToLabelImageFilter
 * \brief Converts a LabelMap to a labeled image.
 *
 * LabelMapToBinaryImageFilter to a label image.
 *
 * \author Gaetan Lehmann. Biologie du Developement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or
 * http://www.insight-journal.org/browse/publication/176
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 * \ingroup LabeledImageFilters
 *
 * \ingroup OTBLabelMap
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT LabelMapToLabelImageFilter : public itk::LabelMapToLabelImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToLabelImageFilter Self;
  typedef itk::LabelMapToLabelImageFilter<TInputImage, TOutputImage> Superclass;
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
  itkTypeMacro(LabelMapToLabelImageFilter, ImageToImageFilter);

protected:
  LabelMapToLabelImageFilter();
  ~LabelMapToLabelImageFilter(){};

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject*){};

private:
  LabelMapToLabelImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end of class

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbLabelMapToLabelImageFilter.hxx"
#endif

#endif
