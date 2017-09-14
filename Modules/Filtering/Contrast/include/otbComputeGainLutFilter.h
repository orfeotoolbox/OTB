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

#ifndef otbComputeGainLutFilter_h
#define otbComputeGainLutFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{

template <class TInputImage , class TOutputImage >
class ITK_EXPORT ComputeGainLutFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public:
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  /** typedef for standard classes. */
  typedef ComputeGainLutFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename InputImageType::PixelType HistoType;

  typedef typename OutputImageType::PixelType LutType;

  typedef typename OutputImageType::InternalPixelType OutputPixelType;

  typedef typename OutputImageType::RegionType OutputImageRegionType;

  itkSetMacro(Min, double);
  itkGetMacro(Min, double);

  itkSetMacro(Max, double);
  itkGetMacro(Max, double);

  itkSetMacro(NbPixel, long);
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeGainLutFilter, ImageToImageFilter);

protected:
  ComputeGainLutFilter();
  ~ComputeGainLutFilter() ITK_OVERRIDE {}

  void BeforeThreadedGenerateData();

  void ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                            itk::ThreadIdType threadId);

  OutputPixelType PostProcess( int countMapValue ,
                               int countValue );

  void Equalized( const HistoType & inputHisto ,
                        HistoType & targetHisto ,
                        LutType & lut);

  void CreateTarget( const HistoType & inputHisto ,
                           HistoType & targetHisto );

  bool IsValide(const HistoType & inputHisto );

private:
  ComputeGainLutFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  int m_NbBin;
  double m_Min;
  double m_Max;
  double m_Step;
  long m_NbPixel;

};

  // End namespace otb
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComputeGainLutFilter.txx"
#endif

#endif