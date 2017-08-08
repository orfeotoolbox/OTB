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

#ifndef otbComputeLutFilter_h
#define otbComputeLutFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{

template <class TInputImage , class TOutputImage >
class ITK_EXPORT ComputeLutFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public:
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  /** typedef for standard classes. */
  typedef ComputeLutFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename InputImageType::InternalPixelType InputPixelType;

  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename TInputImage::PixelType HistoType;

  typedef unsigned int ThreadIdType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeLutFilter, ImageToImageFilter);

  itkSetMacro(Threshold , int);
  itkGetMacro(Threshold , int);

protected:
  ComputeLutFilter();
  ~ComputeLutFilter() ITK_OVERRIDE {}

  // Call  BeforeThreadedGenerateData after getting the number of thread
  void GenerateData();

  void BeforeThreadedGenerateData();

  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            ThreadIdType threadId);

  void AfterThreadedGenerateData();

  void CreateTarget( const HistoType & inputHisto ,
                           HistoType & targetHisto );

  void Equalized( const HistoType & inputHisto ,
                        HistoType & targetHisto ,
                        HistoType & lut);

private:
  ComputeLutFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int m_Threshold;
  int m_NbBin;

};

  // End namespace otb
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComputeLutFilter.txx"
#endif

#endif