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

#ifndef otbApplyGainFilter_h
#define otbApplyGainFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{

/** \class ApplyGainFilter
 *  \brief Apply gain on the input image with a bilineare interpolation
 *
 *  This class implements the third part of the CLAHE algorithm. It's aim
 *  is to apply the computed gain with a bilineare interpolation. The gain
 *  is in a look up table, and the minimum and maximum asked by the filter
 *  should be the same as the one used to compute those look up table.
 *
 * \ingroup OTBContrast
 */

template <class TInputImage, class TLut, class TOutputImage>
class ITK_EXPORT ApplyGainFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef ApplyGainFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TLut                                        LutType;
  typedef typename InputImageType::InternalPixelType  InputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeHistoFilter, ImageToImageFilter);

  /** Get/Set macro to get/set the nodata value */
  itkSetMacro(NoData, InputPixelType);
  itkGetMacro(NoData, InputPixelType);

  /** Get/Set macro to get/set the nodata flag value */
  itkBooleanMacro(NoDataFlag);
  itkGetMacro(NoDataFlag, bool);
  itkSetMacro(NoDataFlag, bool);

  /** Get/Set macro to get/set the ThumbSizeFromSpacing flag value */
  itkBooleanMacro(ThumbSizeFromSpacing);
  itkGetMacro(ThumbSizeFromSpacing, bool);
  itkSetMacro(ThumbSizeFromSpacing, bool);

  /** Get/Set macro to get/set the thumbnail's size */
  itkSetMacro(ThumbSize, typename InputImageType::SizeType);
  itkGetMacro(ThumbSize, typename InputImageType::SizeType);

  /** Get/Set macro to get/set the minimum value */
  itkSetMacro(Min, InputPixelType);
  itkGetMacro(Min, InputPixelType);

  /** Get/Set macro to get/set the maximum value */
  itkSetMacro(Max, InputPixelType);
  itkGetMacro(Max, InputPixelType);

  /** Set the input look up table*/
  void SetInputLut(const LutType* lut);

  /** Set the input image*/
  void SetInputImage(const InputImageType* input);

protected:
  ApplyGainFilter();
  ~ApplyGainFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Get the input image*/
  const InputImageType* GetInputImage() const;

  /** Get the input look up table*/
  const LutType* GetInputLut() const;

  void GenerateInputRequestedRegion() override;

  void BeforeThreadedGenerateData() override;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void VerifyInputInformation() override{};

private:
  ApplyGainFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Bilinear interpolation of the gain between the different window.*/
  double InterpolateGain(typename LutType::ConstPointer gridLut, unsigned int pixelValue, typename InputImageType::IndexType index);

  InputPixelType                    m_NoData;
  InputPixelType                    m_Min;
  InputPixelType                    m_Max;
  bool                              m_NoDataFlag;
  bool                              m_ThumbSizeFromSpacing;
  double                            m_Step;
  typename LutType::SizeType        m_LutSize;
  typename InputImageType::SizeType m_ThumbSize;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbApplyGainFilter.hxx"
#endif


#endif
