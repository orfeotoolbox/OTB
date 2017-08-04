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

#ifndef otbComputeHistoFilter_h
#define otbComputeHistoFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{

template < class TInputImage , class TOutputImage >
class ITK_EXPORT ComputeHistoFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public:
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  /** typedef for standard classes. */
  typedef ComputeHistoFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename InputImageType::InternalPixelType InputPixelType;
  typedef typename InputImageType::IndexType IndexType;
  typedef typename InputImageType::SizeType SizeType;

  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef unsigned int ThreadIdType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeHistoFilter, ImageToImageFilter);

  itkSetMacro(NbBin, int);
  itkGetMacro(NbBin, int);

  itkSetMacro(Min, InputPixelType);
  itkGetMacro(Min, InputPixelType);

  itkSetMacro(Max, InputPixelType);
  itkGetMacro(Max, InputPixelType);

  itkSetMacro(NoData, InputPixelType);
  itkGetMacro(NoData, InputPixelType);

  itkSetMacro(ThumbSize, SizeType);
  itkGetMacro(ThumbSize, SizeType);

  itkGetMacro(TargetHisto, typename OutputImageType::Pointer);

protected:
  ComputeHistoFilter();
  ~ComputeHistoFilter() ITK_OVERRIDE {}

  void GenerateInputRequestedRegion();

  void GenerateOutputInformation();

  // Call  BeforeThreadedGenerateData after getting the number of thread
  void GenerateData();

  void BeforeThreadedGenerateData();

  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            ThreadIdType threadId);

  void AfterThreadedGenerateData();

private:
  ComputeHistoFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename OutputImageType::Pointer m_TargetHisto;
  std::vector< typename OutputImageType::Pointer > m_HistoThread;
  InputPixelType m_Min;
  InputPixelType m_Max;
  InputPixelType m_NoData;
  SizeType m_ThumbSize;
  int m_NbBin;
  double m_Step;



};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComputeHistoFilter.txx"
#endif
  // End namespace otb


#endif