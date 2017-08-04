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

#define DEBUG
namespace otb
{

template <class TInputImage, class TOutputImage >
ComputeHistoFilter < TInputImage , TOutputImage >
::ComputeHistoFilter()
{
  m_TargetHisto =  OutputImageType::New() ;
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
  output->SetLargestPossibleRegion(region);

}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  // Initializing output
  typename OutputImageType::Pointer output = this->GetOutput();
  output->SetVectorLength( m_NbBin );
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
  m_HistoThread.resize(nbThread);
  for (uint i = 0 ; i<nbThread ; i++)
    {
    // typename OutputImageType::Pointer histoThread ( OutputImageType::New() );
    // histoThread->CopyInformation( output );
    // histoThread->SetRequestedRegion( output->GetRequestedRegion() );
    // histoThread->SetBufferedRegion( output->GetBufferedRegion() );
    // histoThread->Allocate();
    // histoThread->FillBuffer( zeroPixel );
    // m_HistoThread.push_back(histoThread);
    m_HistoThread[i] = OutputImageType::New() ;
    m_HistoThread[i]->CopyInformation( output );
    m_HistoThread[i]->SetRequestedRegion( output->GetRequestedRegion() );
    m_HistoThread[i]->SetBufferedRegion( output->GetBufferedRegion() );
    m_HistoThread[i]->Allocate();
    m_HistoThread[i]->FillBuffer( zeroPixel );
    #ifdef DEBUG
    std::cout<<"Thread "<<i<<std::endl;
    std::cout<<"Requested Region "<<m_HistoThread[i]->GetRequestedRegion().GetSize()<<std::endl;
    std::cout<<"Largest Region "<<m_HistoThread[i]->GetLargestPossibleRegion().GetSize()<<std::endl;    
    std::cout<<"Buffered Region "<<m_HistoThread[i]->GetBufferedRegion().GetSize()<<std::endl;
    std::cout<<"Vector length "<<m_HistoThread[i]->GetVectorLength()<<std::endl;
    #endif
    } 

  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( m_NbBin -1 );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread ,
                       ThreadIdType threadId )
{
  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = m_HistoThread[threadId];
  typename itk::ImageRegionIterator < OutputImageType > 
      oit( output , output->GetRequestedRegion() );
  typename InputImageType::RegionType region;
  int pixel = 0;
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    IndexType start;
    start[0] = oit.GetIndex()[0] * m_ThumbSize[0];
    start[1] = oit.GetIndex()[1] * m_ThumbSize[1];
    region.SetIndex(start);
    region.SetSize(m_ThumbSize);
    typename itk::ImageRegionConstIterator < InputImageType > 
      it( input ,region );
    it.GoToBegin();
    while ( !it.IsAtEnd() )
      {
      if( it.Get() == m_NoData )
        {
        ++it;
        continue;
        }
      pixel = static_cast<int>( std::round( ( it.Get() - m_Min ) / m_Step ) );
      ++oit.Get()[pixel];
      ++it;
      }
    ++oit;
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
  std::vector< typename itk::ImageRegionIterator < OutputImageType > > threadIt;
  int agreg = 0;
  for (uint threadId = 0 ; threadId<nbThread ; threadId++ )
    {
    threadIt[threadId] = itk::ImageRegionIterator < OutputImageType > \
      ( m_HistoThread[threadId] , m_HistoThread[threadId]->GetRequestedRegion() );
    threadIt[threadId].GoToBegin();    
    }
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
    for (int i = 0 ; i<m_NbBin ; i++)
      {
      agreg = 0;
      for (uint threadId = 0 ; threadId<nbThread ; threadId++ )
        {
        agreg += threadIt[threadId].Get()[i]; 
        }
      oit.Get()[i] = agreg;
      }
    ++oit;
    for (uint threadId = 0 ; threadId<nbThread ; threadId++ )
      {
      threadIt[threadId].GoToBegin();    
      }
    }
}


  // End namespace otb
}

#endif