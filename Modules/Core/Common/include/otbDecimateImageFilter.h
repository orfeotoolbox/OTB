/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbDecimateImageFilter_h
#define otbDecimateImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

namespace otb
{

/** \class DecimateImageFilter
 * \brief Performs a down sampling of an image
 *
 * This class performs a basic down sampling of an image. On the contrary to
 * itk::ResampleImageFilter, it does not perform any interpolation.
 *
 * \sa ResampleImageFilter
 * \sa SubsampleImageRegionConstIterator
 *
 * \ingroup OTBCommon
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DecimateImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  using Self = DecimateImageFilter;
  using Superclass = itk::ImageToImageFilter<TInputImage, TOutputImage>;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DecimateImageFilter, ImageToImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Image typedef support. */
  using InputImageType = TInputImage;
  using InputImageRegionType = typename InputImageType::RegionType;

  using OutputImageType = TOutputImage;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputPixelType = typename OutputImageType::PixelType;

  /** Set/Get the DecimateFactor */
  itkGetMacro(DecimationFactor, unsigned int);
  itkSetMacro(DecimationFactor, unsigned int);

protected:
  DecimateImageFilter()
  {
    m_DecimationFactor = 1;
    this->DynamicMultiThreadingOn();
  }
  virtual ~DecimateImageFilter() = default;

  /** Since input and output image are very likely to be of different size.
   * Region estimation functions has to be reimplemented
   */
  void CallCopyOutputRegionToInputRegion(InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion) override;
  void CallCopyInputRegionToOutputRegion(OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion) override;

  /** Output image region size is not of the same dimension as the input.
   * That is why GenerateOutputInformation has to be redefined.
   */
  void GenerateOutputInformation() override;

  /** Allows multithreading */
  void DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread) override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  DecimateImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_DecimationFactor;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDecimateImageFilter.hxx"
#endif

#endif
