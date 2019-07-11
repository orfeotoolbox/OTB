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

#ifndef otbClosingOpeningMorphologicalFilter_h
#define otbClosingOpeningMorphologicalFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/**
 * \class ClosingOpeningMorphologicalFilter
 * \brief This filter implements an opening grayscale morphological operation
 * followed by a closing grayscale morphological operation. It tends to simplify
 * image by obliterating details with a lower extent than the structuring element.
 *
 * This filter implements an opening grayscale morphological operation
 * followed by a closing grayscale morphological operation. It tends to simplify
 * image by obliterating details with a lower extent than the structuring element.
 *
 * This filter can be used in the morphological pyramid analyse filter. Due to the order
 * of composition of the two basic morphological operation, the filtered details are dark
 * on a brighter background.
 *
 * \sa ClosingOpeningMorphologicalFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT ClosingOpeningMorphologicalFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ClosingOpeningMorphologicalFilter                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(ClosingOpeningMorphologicalFilter, ImageToImageFilter);

  /** Template parameter typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TKernel      KernelType;

  /** Useful typedefs */
  typedef typename InputImageType::Pointer     InputImagePointer;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename TInputImage::PixelType      PixelType;

  /** Kernel accessors */
  itkSetMacro(Kernel, KernelType);
  itkGetConstReferenceMacro(Kernel, KernelType);

protected:
  /** Constructor */
  ClosingOpeningMorphologicalFilter();
  /** Destructor */
  ~ClosingOpeningMorphologicalFilter() override {}

  /* void GenerateInputRequestedRegion(); */
  /*   void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output)); */

  /** Main computation method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ClosingOpeningMorphologicalFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  /** Morphological operation kernel  */
  KernelType m_Kernel;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClosingOpeningMorphologicalFilter.hxx"
#endif
#endif
