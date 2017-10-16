/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
 *  \brief 
 * \ingroup OTBContrast
 */

template < class TInputImage , class TOutputImage >
class ITK_EXPORT CLHistogramEqualizationFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public :
  /** typedef for standard classes. */

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  typedef CLHistogramEqualizationFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef otb::VectorImage< int , 2 > HistogramType;

  typedef itk::StreamingImageFilter< HistogramType , HistogramType >
    StreamingImageFilter;

  typedef otb::InPlacePassFilter < InputImageType > BufferFilter;

  typedef otb::ComputeHistoFilter< InputImageType , HistogramType >
    HistoFilter;

  typedef otb::ComputeGainLutFilter< HistogramType , HistogramType >
    GainLutFilter;

  typedef otb::ApplyGainFilter< InputImageType , HistogramType , OutputImageType >
    ApplyGainFilter;

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self)
  /** Run-time type information (and related methods). */
  itkTypeMacro(CLHistogramEqualizationFilter, ImageToImageFilter)

  itkGetMacro(Min, InputPixelType)
  itkSetMacro(Min, InputPixelType)

  itkGetMacro(Max, InputPixelType)
  itkSetMacro(Max, InputPixelType)

  itkGetMacro(NbBin, unsigned long)
  itkSetMacro(NbBin, unsigned long)

  itkSetMacro(ThumbSize, typename InputImageType::SizeType)
  itkGetMacro(ThumbSize, typename InputImageType::SizeType)

  itkGetMacro(Threshold, double)
  itkSetMacro(Threshold, double)

  itkGetMacro(NoData, InputPixelType)
  itkSetMacro(NoData, InputPixelType)

  itkBooleanMacro(NoDataFlag)
  itkGetMacro(NoDataFlag, bool)
  itkSetMacro(NoDataFlag, bool)


protected :
  CLHistogramEqualizationFilter();
  ~CLHistogramEqualizationFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override ;

  void BeforeThreadedGenerateData() override;
  
  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            itk::ThreadIdType threadId) override;

private :
  CLHistogramEqualizationFilter(const Self &) = delete ;
  void operator =(const Self&) = delete ;

  typename HistoFilter::Pointer m_HistoFilter;
  typename GainLutFilter::Pointer m_GainLutFilter;
  typename ApplyGainFilter::Pointer m_ApplyGainFilter;
  typename StreamingImageFilter::Pointer m_StreamingImageFilter;
  typename BufferFilter::Pointer m_BufferFilter;
  InputPixelType m_Min , m_Max , m_NoData;
  unsigned long m_NbBin;
  typename InputImageType::SizeType m_ThumbSize;
  double m_Threshold , m_Step;
  bool m_NoDataFlag;

};

}  // End namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCLHistogramEqualizationFilter.txx"
#endif


#endif
