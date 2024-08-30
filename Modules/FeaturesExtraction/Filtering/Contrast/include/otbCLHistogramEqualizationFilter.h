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

#ifndef otbCLHistogramEqualizationFilter_h
#define otbCLHistogramEqualizationFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbComputeHistoFilter.h"
#include "otbComputeGainLutFilter.h"
#include "otbApplyGainFilter.h"
#include "itkStreamingImageFilter.h"
#include "otbInPlacePassFilter.h"

namespace otb
{

/** \class CLHistogramEqualizationFilter
 *  \brief Implement CLAHE algorithm
 *
 *  This class implement CLAHE algorithm. It is a composite filter that gathers
 *  the 3 filters (ComputeHisto, ComputeGainLut, ApplyGain) and pipes them with
 *  additional filters (InPlacePass and StreamingImage) in order to make
 *  streaming available.
 * \ingroup OTBContrast
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT CLHistogramEqualizationFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef CLHistogramEqualizationFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::VectorImage<unsigned int, 2> HistogramType;
  typedef otb::VectorImage<double, 2>       LutType;

  typedef itk::StreamingImageFilter<LutType, LutType> StreamingImageFilter;

  typedef otb::InPlacePassFilter<InputImageType> BufferFilter;

  typedef otb::ComputeHistoFilter<InputImageType, HistogramType> HistoFilter;

  typedef otb::ComputeGainLutFilter<HistogramType, LutType> GainLutFilter;

  typedef otb::ApplyGainFilter<InputImageType, LutType, OutputImageType> ApplyGainFilter;

  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(CLHistogramEqualizationFilter, ImageToImageFilter);

  itkGetMacro(Min, InputPixelType);
  // itkSetMacro(Min, InputPixelType);
  void SetMin(InputPixelType min)
  {
    m_HistoFilter->SetMin(min);
    m_GainLutFilter->SetMin(min);
    m_ApplyGainFilter->SetMin(min);
    m_Min = min;
  };

  itkGetMacro(Max, InputPixelType);
  // itkSetMacro(Max, InputPixelType);
  void SetMax(InputPixelType max)
  {
    m_HistoFilter->SetMax(max);
    m_GainLutFilter->SetMax(max);
    m_ApplyGainFilter->SetMax(max);
    m_Max = max;
  };

  itkGetMacro(NbBin, unsigned long);
  // itkSetMacro(NbBin, unsigned long);
  void SetNbBin(unsigned long bin)
  {
    m_HistoFilter->SetNbBin(bin);
    m_NbBin = bin;
  };

  itkGetMacro(ThumbSize, typename InputImageType::SizeType);
  // itkSetMacro(ThumbSize, typename InputImageType::SizeType);
  void SetThumbSize(typename InputImageType::SizeType size)
  {
    m_HistoFilter->SetThumbSize(size);
    m_ApplyGainFilter->SetThumbSize(size);
    m_GainLutFilter->SetNbPixel(size[0] * size[1]);
    m_ThumbSize = size;
  };

  itkGetMacro(Threshold, double);
  // itkSetMacro(Threshold, double);
  void SetThreshold(double t)
  {
    m_HistoFilter->SetThreshold(t);
    m_Threshold = t;
  };

  itkGetMacro(NoData, InputPixelType);
  // itkSetMacro(NoData, InputPixelType);
  void SetNoData(InputPixelType n)
  {
    m_HistoFilter->SetNoData(n);
    m_ApplyGainFilter->SetNoData(n);
    m_NoData = n;
  }


  itkGetMacro(NoDataFlag, bool);
  // itkSetMacro(NoDataFlag, bool);
  void SetNoDataFlag(bool flag)
  {
    m_HistoFilter->SetNoDataFlag(flag);
    m_ApplyGainFilter->SetNoDataFlag(flag);
    m_NoDataFlag = flag;
  }

protected:
  CLHistogramEqualizationFilter();
  ~CLHistogramEqualizationFilter() override
  {
  }

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void UpdateOutputInformation() override;

  void PropagateRequestedRegion(itk::DataObject* output) override;

  void GenerateData() override;

private:
  CLHistogramEqualizationFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  typename HistoFilter::Pointer          m_HistoFilter;
  typename GainLutFilter::Pointer        m_GainLutFilter;
  typename ApplyGainFilter::Pointer      m_ApplyGainFilter;
  typename StreamingImageFilter::Pointer m_StreamingImageFilter;
  typename BufferFilter::Pointer         m_BufferFilter;
  InputPixelType                         m_Min, m_Max, m_NoData;
  unsigned long                          m_NbBin;
  typename InputImageType::SizeType      m_ThumbSize{0,0};
  double                                 m_Threshold;
  bool                                   m_NoDataFlag;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCLHistogramEqualizationFilter.hxx"
#endif


#endif
