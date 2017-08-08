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

#ifndef otbComputeLutFilter_txx
#define otbComputeLutFilter_txx

#include "otbComputeLutFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkImageRegionIterator.h"

#include <limits>


namespace otb
{

template < class TInputImage, class TOutputImage >
ComputeLutFilter < TInputImage , TOutputImage >
::ComputeLutFilter()
{
  m_NbBin = 256;
  m_Threshold = 3;
}

template <class TInputImage, class TOutputImage >
void ComputeLutFilter < TInputImage , TOutputImage >
::GenerateData()
{
  #ifdef DEBUG
  std::cout<<"output requested "<<this->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;
  std::cout<<"output Largest "<<this->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
  std::cout<<"output Buffered "<<this->GetOutput()->GetBufferedRegion().GetSize()<<std::endl;
  #endif
  this->AllocateOutputs();
  #ifdef DEBUG
  std::cout<<"After Allocate"<<std::endl;
  std::cout<<"output requested "<<this->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;
  std::cout<<"output Largest "<<this->GetOutput()->GetLargestPossibleRegion().GetSize()<<std::endl;
  std::cout<<"output Buffered "<<this->GetOutput()->GetBufferedRegion().GetSize()<<std::endl;
  #endif

  // Set up the multithreaded processing
  typename itk::ImageSource<OutputImageType>::ThreadStruct str;
  str.Filter = this;

  // Get the output pointer
  const OutputImageType *outputPtr = this->GetOutput();
  const itk::ImageRegionSplitterBase * splitter = this->GetImageRegionSplitter();
  const unsigned int validThreads = 
    splitter->GetNumberOfSplits( outputPtr->GetRequestedRegion() , 
                                 this->GetNumberOfThreads() );

  this->BeforeThreadedGenerateData();

  this->GetMultiThreader()->SetNumberOfThreads( validThreads );
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  this->AfterThreadedGenerateData();
}

template <class TInputImage , class TOutputImage >
void ComputeLutFilter <TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  m_NbBin = this->GetInput()->GetNumberOfComponentsPerPixel();
}

template <class TInputImage , class TOutputImage >
void ComputeLutFilter <TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  typename InputImageType::ConstPointer input = this->GetInput();
  typename InputImageType::Pointer output = this->GetOutput();

  typename InputImageType::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread , outputRegionForThread);
  // Is it usefull???

  // support progress methods/callbacks
  itk::ProgressReporter progress(this , threadId , 
                outputRegionForThread.GetNumberOfPixels() );

  itk::ImageRegionConstIterator < InputImageType > it ( input , 
                                                        inputRegionForThread );

  itk::ImageRegionIterator <OutputImageType > oit ( output ,
                                                    outputRegionForThread );
  HistoType target , lut ;
  target.SetSize( m_NbBin );
  lut.SetSize( m_NbBin );
  it.GoToBegin();
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
      target.Fill(0);
      lut.Fill(0);
      CreateTarget( it.Get() , target );
      Equalized( it.Get() , target , lut );
      oit.Set(lut);
      ++oit;
      ++it;
    }
}

template <class TInputImage , class TOutputImage >
void ComputeLutFilter <TInputImage , TOutputImage >
::AfterThreadedGenerateData()
{

}

template <class TInputImage, class TOutputImage >
void ComputeLutFilter < TInputImage , TOutputImage >
::Equalized( const HistoType & inputHisto ,
             HistoType & targetHisto ,
             HistoType & lut)
{
  int countMapValue = 0;
  int countValue = 0;
  lut[countValue] = 0; // Black stays black
  ++countValue;
  int countInput  = inputHisto[ 0 ] + inputHisto[ countValue ];
  lut[m_NbBin - 1 ] = m_NbBin - 1 ; // White stays white
  int countTarget = targetHisto[ countMapValue ];

  while ( countMapValue< (m_NbBin) && countValue< ( m_NbBin - 1 ) )
    {
    if (countInput > countTarget)
      {
      ++countMapValue;
      countTarget += targetHisto[ countMapValue ];
      }
    else
      { 
      lut[countValue] =  countMapValue ;
      ++countValue;
      countInput  += inputHisto[ countValue ];
      }
    }
  for (uint i = 0 ; i < m_NbBin ; i++)
    {
    if (lut[i] == -1)
      {
      lut[i] = lut[i-1];  
      } 
    }
}

template <class TInputImage, class TOutputImage >
void ComputeLutFilter < TInputImage , TOutputImage >
::CreateTarget( const HistoType & inputHisto ,
                HistoType & targetHisto )
{
  int nbPixel = 0;
  for ( int i = 0 ; i < m_NbBin ; i++ )
    {
    nbPixel += inputHisto[i];
    }
  int rest = nbPixel % m_NbBin;
  int height = nbPixel / m_NbBin;
  targetHisto.Fill(height);
  for ( int i = 0 ; i < rest ; i++ )
    {
    ++targetHisto[(m_NbBin - rest)/2 + i];
    }  
}
  // End namespace otb
}

#endif