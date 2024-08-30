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

#ifndef otbTwoNRIBandsImageToNComplexBandsImage_h
#define otbTwoNRIBandsImageToNComplexBandsImage_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

/*
 * Inputs : one single image made of 2N real bands
 * Output : one single image made of N complex bands
 *
 * */


namespace otb
{


template <class TInputImage, class TOutputImage>
class ITK_EXPORT TwoNRIBandsImageToNComplexBandsImage : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract input and output image dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** standard class typedefs */
  typedef TwoNRIBandsImageToNComplexBandsImage Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Object factory management */
  itkNewMacro(Self);

  /** typemacro */
  itkTypeMacro(TwoNRIBandsImageToNComplexBandsImage, ImageToImageFilter);

  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;


protected:
  TwoNRIBandsImageToNComplexBandsImage();
  ~TwoNRIBandsImageToNComplexBandsImage() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateOutputInformation(void) override;
  void BeforeThreadedGenerateData(void) override;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  TwoNRIBandsImageToNComplexBandsImage(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTwoNRIBandsImageToNComplexBandsImage.hxx"
#endif

#endif
