/*
 * Copyright(C) 2005-2020 Centre National d'Etudes Spatiales(CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https: //www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbClampROIFilter_h
#define otbClampROIFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/**
 * Region clamping filter.
 * This filter is a kind of ROI pass filter. Data within the ROI is kept with
 * its original value. Data outside ROI is forced to 0.
 *
 * Also, this filter propagate the exact ROI upstream in the pipeline. This
 * way, if it's piped after another filter, the upstream filter isn't executed
 * on the data outside the ROI.
 *
 * \tparam TImage  Image type.
 * \sa `otb::ExtractROI<>`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 */
template <typename TImage>
class ClampROIFilter : public itk::ImageToImageFilter<TImage, TImage>
{
public:

  /**\name Convenient typedefs for simplifying declarations */
  //@{
  using InputImageType  = TImage;
  using OutputImageType = TImage;
  //@}

  /**\name Extract dimension from input and output images */
  //@{
  itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);
  //@}
  /**\name Standard class typedefs */
  //@{
  using Self            = ClampROIFilter;
  using Superclass      = itk::ImageToImageFilter<InputImageType, OutputImageType>;
  using Pointer         = itk::SmartPointer<Self>;
  using ConstPointer    = itk::SmartPointer<const Self>;
  //@}

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClampROIFilter, unused);

  /**\name Image typedef support */
  //@{
  using InputPixelType        = typename InputImageType::PixelType;
  using OutputPixelType       = typename OutputImageType::PixelType;
  using InputRealType         = typename itk::NumericTraits<InputPixelType>::RealType;
  using InputImageRegionType  = typename InputImageType::RegionType;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using InputIndexType        = typename InputImageType::IndexType;
  using InputSizeType         = typename InputImageType::SizeType;
  using OutputIndexType       = typename OutputImageType::IndexType;
  using OutputSizeType        = typename OutputImageType::SizeType;

  static_assert(InputImageDimension == OutputImageDimension, "Images have the same number of components");
   //@}

  /** Column threshold setter. */
  void SetThresholdX(long threshold) noexcept
  { m_thresholdX = threshold; }
  /** Column threshold getter. */
  long GetThresholdX() const noexcept
  { return m_thresholdX;}

  /** Top line threshold setter. */
  void SetThresholdYtop(long threshold) noexcept
  { m_thresholdYtop = threshold; }
  /** Top line threshold getter. */
  long GetThresholdYtop() const noexcept
  { return m_thresholdYtop;}

  /** Bottom line threshold setter. */
  void SetThresholdYbot(long threshold) noexcept
  { m_thresholdYbot = threshold; }
  /** Bottom line threshold getter. */
  long GetThresholdYbot() const noexcept
  { return m_thresholdYbot;}

protected:
  /// Hidden constructor
  ClampROIFilter() = default;

  InputImageType      * GetInputImage()       { return const_cast<InputImageType*>(this->GetInput()); }
  InputImageType const* GetInputImage() const { return this->GetInput(); }

  /** otbClampROIFilter doesn't need an input requested region as large as the
   * output requested region.
   * \sa ImageToImageFilter::GenerateInputRequestedRegion()
   */
  void CallCopyOutputRegionToInputRegion(
      InputImageRegionType       & destRegion,
      OutputImageRegionType const& srcRegion) override
  {
    destRegion = OutputRegionToInputRegion(srcRegion);
  }

  /**
   * Functional implementation of `CallCopyOutputRegionToInputRegion()`.
   */
  InputImageRegionType OutputRegionToInputRegion(
      OutputImageRegionType const& srcRegion);

  /**
   * Main computation function called by each thread.
   * \param[in] outputRegionForThread  Specified output region to compute
   * \param[in] threadId               Id of the computing threads
   */
  void ThreadedGenerateData(
      OutputImageRegionType const& outputRegionForThread,
      itk::ThreadIdType            threadId) override;

private:
  long m_thresholdX    = 0;
  long m_thresholdYtop = 0;
  long m_thresholdYbot = 0;
};

} // otb namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClampROIFilter.hxx"
#endif

#endif  // otbClampROIFilter_h
