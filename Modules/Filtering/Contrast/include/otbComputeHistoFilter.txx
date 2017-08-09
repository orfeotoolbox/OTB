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

#ifndef otbComputeHistoFilter_txx
#define otbComputeHistoFilter_txx

#include "otbComputeHistoFilter.h"
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"

#include <limits>

// #define DEBUG
namespace otb
{

template <class TInputImage, class TOutputImage >
ComputeHistoFilter < TInputImage , TOutputImage >
::ComputeHistoFilter()
{
  //m_TargetHisto =  OutputImageType::New() ;
  m_Min = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_Max = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NoData = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NbBin = 256;
  m_ThumbSize.Fill(-1);
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
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


template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateInputRequestedRegion()
{
  assert( m_ThumbSize[0]>0);
  assert( m_ThumbSize[1]>0);

  Superclass::GenerateInputRequestedRegion();
  typename Superclass::InputImagePointer inputPtr =
                  const_cast<InputImageType *>(this->GetInput());
  IndexType start ;
  start[0] = this->GetOutput()->GetRequestedRegion().GetIndex()[0] * m_ThumbSize[0];
  start[1] = this->GetOutput()->GetRequestedRegion().GetIndex()[1] * m_ThumbSize[1];
  SizeType size ;
  size[0] = this->GetOutput()->GetRequestedRegion().GetSize()[0] * m_ThumbSize[0];
  size[1] = this->GetOutput()->GetRequestedRegion().GetSize()[1] * m_ThumbSize[1];

#ifdef DEBUG
  std::cout<<"start "<<start<<std::endl;
  std::cout<<"size "<<size<<std::endl;
  std::cout<<"Requested Index  "<<this->GetOutput()->GetRequestedRegion().GetIndex()<<std::endl;
  std::cout<<"Requested Size "<<this->GetOutput()->GetRequestedRegion().GetSize()<<std::endl;
#endif

  typename InputImageType::RegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(start);
  inputRequestedRegion.SetSize(size);

  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = this->GetOutput();

  if ( !input || !output )
    {
    return;
    }

  typename OutputImageType::IndexType start;
  typename OutputImageType::SizeType size;
  start[0] = 0;
  start[1] = 0;
  size[0] = input->GetLargestPossibleRegion().GetSize()[0]/m_ThumbSize[0];
  size[1] = input->GetLargestPossibleRegion().GetSize()[1]/m_ThumbSize[1];
  typename OutputImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  output->SetNumberOfComponentsPerPixel(m_NbBin);
  output->SetLargestPossibleRegion(region);

}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  // Initializing output
  typename OutputImageType::Pointer output = this->GetOutput();
  typename OutputImageType::PixelType zeroPixel ; 
  zeroPixel.SetSize( m_NbBin );
  zeroPixel.Fill(0);
  #ifdef DEBUG
  std::cout<<"Vector length "<<output->GetVectorLength()<<std::endl;
  std::cout<<"zeroPixel length "<<zeroPixel.GetSize()<<std::endl;
  #endif
  output->FillBuffer( zeroPixel );

  // Initializing shared variable with thread number parameter
  ThreadIdType const nbThread = this->GetNumberOfThreads();
  SizeType outSize = output->GetRequestedRegion().GetSize();
  m_HistoThread.resize( nbThread*outSize[0]*outSize[1] , zeroPixel );

  #ifdef DEBUG
  std::cout<<"NbThread "<<nbThread<<std::endl;
  std::cout<<"vector size "<<m_HistoThread.size()<<std::endl;
  std::cout<<"Vector's pixel "<<m_HistoThread[0]<<std::endl;
  #endif

  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( m_NbBin -1 );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & itkNotUsed(outputRegionForThread) ,
                       ThreadIdType threadId )
{
  typename InputImageType::ConstPointer input = this->GetInput();
  SizeType outSize = this->GetOutput()->GetRequestedRegion().GetSize();
  IndexType outIndex = this->GetOutput()->GetRequestedRegion().GetIndex();
  int threadIndex = threadId * outSize[0] * outSize[1];
  typename InputImageType::RegionType region;
  int pixel = 0;
  for ( uint nthHisto = 0 ; nthHisto < outSize[0] * outSize[1] ; nthHisto++ )
    {
    IndexType start;
    start[0] = (outIndex[0] +  nthHisto % outSize[0] ) * m_ThumbSize[0];
    start[1] = (outIndex[1] +  nthHisto / outSize[0] ) * m_ThumbSize[1];
    #ifdef DEBUG
    std::cout<<"Region start threaded "<<start<<std::endl;
    #endif
    region.SetIndex(start);
    region.SetSize(m_ThumbSize);
    typename itk::ImageRegionConstIterator < InputImageType > 
      it( input ,region );
    it.GoToBegin();
    while ( !it.IsAtEnd() )
      {
      if( it.Get() == m_NoData || it.Get() > m_Max || it.Get() < m_Min )
        {
        ++it;
        continue;
        }
      pixel = static_cast<int>( std::round( ( it.Get() - m_Min ) / m_Step ) );
      ++m_HistoThread[threadIndex + nthHisto][pixel];
      ++it;
      }
    }
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::AfterThreadedGenerateData()
{
  ThreadIdType const nbThread = this->GetNumberOfThreads();
  typename OutputImageType::Pointer output = this->GetOutput();
  typename itk::ImageRegionIterator < OutputImageType > 
      oit( output , output->GetRequestedRegion() );
  SizeType outSize = this->GetOutput()->GetRequestedRegion().GetSize();
  IndexType outIndex = this->GetOutput()->GetRequestedRegion().GetIndex();
  int agreg = 0;
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    for (int i = 0 ; i<m_NbBin ; i++)
      {
      agreg = 0;
      for (uint threadId = 0 ; threadId<nbThread ; threadId++ )
        {
        agreg += m_HistoThread[threadId * outSize[0] * outSize[1] \
          + ( ( oit.GetIndex()[0] - outIndex[0] )  ) \
          + ( oit.GetIndex()[1] - outIndex[1] ) * outSize[0]][i]; 
          // std::cout<<threadId * outSize[0] * outSize[1] + ( ( oit.GetIndex()[0] - outIndex[0] )  )  + ( oit.GetIndex()[1] - outIndex[1] ) * outSize[0]<<std::endl;
          // std::cout<<agreg<<std::endl;
        }
      oit.Get()[i] = agreg;
      }
    ++oit;
    }
}


  
} // End namespace otb

#endif