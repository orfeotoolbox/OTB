/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbComputeHistoFilter_hxx
#define otbComputeHistoFilter_hxx

#include "otbComputeHistoFilter.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT

#include <limits>
namespace otb
{

template <class TInputImage, class TOutputImage >
ComputeHistoFilter < TInputImage , TOutputImage >
::ComputeHistoFilter()
{
  OTB_DISABLE_DYNAMIC_MT
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput( 0, this->MakeOutput(0) );
  this->SetNthOutput( 1, this->MakeOutput(1) );
  m_Min = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_Max = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NoDataFlag = false;
  m_NoData = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NbBin = 256;
  m_Threshold = -1;
  m_ThumbSize.Fill(0);
  m_ValidThreads = 1;
  m_Step = -1;
}

template <class TInputImage, class TOutputImage >
itk::ProcessObject::DataObjectPointer 
ComputeHistoFilter < TInputImage , TOutputImage >
::MakeOutput(itk::ProcessObject::DataObjectPointerArraySizeType idx)
{
  itk::DataObject::Pointer output;
 
  switch ( idx )
    {
    case 0:
      output = ( OutputImageType::New() ).GetPointer();
      break;
    case 1:
      output = ( OutputImageType::New() ).GetPointer();
      break;
    default:
      std::cerr << "No output " << idx << std::endl;
      output = NULL;
      break;
    }
  return output;
}

template <class TInputImage, class TOutputImage >
itk::ProcessObject::DataObjectPointer 
ComputeHistoFilter < TInputImage , TOutputImage >
::MakeOutput(const itk::ProcessObject::DataObjectIdentifierType & name)
{
  return Superclass::MakeOutput( name );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateInputRequestedRegion()
{
  typename Superclass::InputImagePointer inputPtr (
                  const_cast<InputImageType *>( this->GetInput() ) );
  inputPtr->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );
  if ( inputPtr->GetRequestedRegion().GetNumberOfPixels() == 0 )
  {
  inputPtr->SetRequestedRegionToLargestPossibleRegion();
  }
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  typename InputImageType::ConstPointer input ( this->GetInput() );
  typename OutputImageType::Pointer output ( this->GetHistoOutput() );
  typename OutputImageType::Pointer outImage ( this->GetOutput() );

  typename InputImageType::RegionType inputLargestRegion (
        input->GetLargestPossibleRegion());
  outImage->SetLargestPossibleRegion( inputLargestRegion );

  typename OutputImageType::IndexType start;
  typename OutputImageType::SizeType size;

  start.Fill(0);

  assert( m_ThumbSize[0] != 0 );
  assert( m_ThumbSize[1] != 0 );

  //TODO throw error if 0

  size[0] = std::ceil( inputLargestRegion.GetSize()[0] / 
        static_cast< double > ( m_ThumbSize[0] ) );
  size[1] = std::ceil( inputLargestRegion.GetSize()[1] / 
        static_cast< double > ( m_ThumbSize[1] ) );

  typename OutputImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  output->SetNumberOfComponentsPerPixel(m_NbBin);
  output->SetLargestPossibleRegion(region);
  typename InputImageType::SpacingType inputSpacing ( input->GetSignedSpacing() );
  typename InputImageType::PointType inputOrigin ( input->GetOrigin() );

  typename OutputImageType::SpacingType histoSpacing ;
  histoSpacing[0] = inputSpacing[0] * m_ThumbSize[0] ;
  histoSpacing[1] = inputSpacing[1] * m_ThumbSize[1] ;
  output->SetSignedSpacing( histoSpacing ) ;

  typename OutputImageType::PointType histoOrigin ;
  histoOrigin[0] = histoSpacing[0] / 2 +  inputOrigin[0] - inputSpacing[0] / 2 ;
  histoOrigin[1] = histoSpacing[1] / 2 +  inputOrigin[1] - inputSpacing[1] / 2 ;
  output->SetOrigin( histoOrigin );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateOutputRequestedRegion( itk::DataObject * itkNotUsed(output) )
{
  if ( GetHistoOutput()->GetRequestedRegion().GetNumberOfPixels() == 0 )
    {
    GetHistoOutput()->SetRequestedRegionToLargestPossibleRegion();
    }
  typename OutputImageType::Pointer outImage ( this->GetOutput() );
  SetRequestedRegion( outImage );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::GenerateData()
{
  this->AllocateOutputs();

  // Set up the multithreaded processing
  typename itk::ImageSource<OutputImageType>::ThreadStruct str;
  str.Filter = this;

  // Get the output pointer
  const OutputImageType * outputPtr ( this->GetOutput() );
  const itk::ImageRegionSplitterBase * splitter ( 
    this->GetImageRegionSplitter() );
  m_ValidThreads = 
    splitter->GetNumberOfSplits( outputPtr->GetRequestedRegion() , 
                                 this->GetNumberOfThreads() );

  this->BeforeThreadedGenerateData();

  this->GetMultiThreader()->SetNumberOfThreads( m_ValidThreads );
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);
  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  this->AfterThreadedGenerateData();
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  // Initializing output
  typename OutputImageType::Pointer output ( this->GetHistoOutput() );
  typename OutputImageType::PixelType zeroPixel(m_NbBin) ;
  zeroPixel.Fill(0);
  output->FillBuffer( zeroPixel );

  // Initializing shared variable with thread number parameter
  SizeType outSize ( output->GetRequestedRegion().GetSize() );
  m_HistoThread.resize( m_ValidThreads * outSize[0] * outSize[1] );
  m_HistoThread.shrink_to_fit();
  std::fill( m_HistoThread.begin() , m_HistoThread.end() , zeroPixel );

  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( m_NbBin -1 );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread ,
                       itk::ThreadIdType threadId )
{
  assert(m_Step>0);
  // TODO throw error

  // itk::ProgressReporter progress(this , threadId , 
  //               outputRegionForThread.GetNumberOfPixels() );
  typename InputImageType::ConstPointer input ( this->GetInput() );
  typename OutputImageType::Pointer output ( this->GetHistoOutput() );

  OutputImageRegionType histoRegion ( 
                            GetHistoOutput()->GetRequestedRegion() );
  SizeType outSize ( histoRegion.GetSize() );
  IndexType outIndex ( histoRegion.GetIndex() );
  
  typename InputImageType::RegionType region;

  unsigned int threadIndex ( threadId  * outSize[0] * outSize[1] ) , pixel(0) ;

  for ( unsigned int nthHisto = 0 ;
        nthHisto < outSize[0] * outSize[1] ; nthHisto++ )
    {
    IndexType start;
    start[0] = ( outIndex[0] +  nthHisto % outSize[0] ) * m_ThumbSize[0];
    start[1] = ( outIndex[1] +  nthHisto / outSize[0] ) * m_ThumbSize[1];
    region.SetSize( m_ThumbSize );
    region.SetIndex(start);

    if ( !region.Crop( outputRegionForThread ) )
      continue;

    typename itk::ImageRegionConstIterator < InputImageType > 
      it( input , region );
    InputPixelType currentPixel(0);
    for ( it.GoToBegin() ; !it.IsAtEnd() ; ++it )
      {
      currentPixel = it.Get();
      if( ( currentPixel == m_NoData && m_NoDataFlag ) || 
            currentPixel > m_Max || currentPixel < m_Min )
        continue;

      pixel = static_cast< unsigned int >( 
        std::round( ( currentPixel - m_Min ) / m_Step ) );
      ++m_HistoThread[threadIndex + nthHisto][pixel];
      }
    
    }
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::AfterThreadedGenerateData()
{
  typename OutputImageType::Pointer output ( this->GetHistoOutput() );
  typename itk::ImageRegionIterator < OutputImageType > 
      oit( output , output->GetRequestedRegion() );
  OutputImageRegionType histoRegion ( 
                        GetHistoOutput()->GetRequestedRegion() );
  SizeType outSize ( histoRegion.GetSize() );
  IndexType outIndex ( histoRegion.GetIndex() );

  unsigned int agreg(0) , total(0) ;

  for ( oit.GoToBegin() ; !oit.IsAtEnd() ; ++oit )
    {
    total = 0;

    for ( unsigned int i = 0 ; i < m_NbBin ; i++ )
      {
      agreg = 0;

      for ( unsigned int threadId = 0 ; threadId < m_ValidThreads ; threadId++ )
        {
        agreg += m_HistoThread[ threadId * outSize[0] * outSize[1]
          + ( oit.GetIndex()[1] - outIndex[1] ) * outSize[0] 
          + ( ( oit.GetIndex()[0] - outIndex[0] ) ) ][i] ;
        }
      oit.Get()[i] = agreg;
      total += agreg;
      }
    if ( m_Threshold > 0 )
      ApplyThreshold( oit , total );
    }
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::ApplyThreshold( typename itk::ImageRegionIterator < OutputImageType > oit ,
                  unsigned int total )
{
  unsigned int rest(0);
  unsigned int height ( static_cast<unsigned int>( 
        m_Threshold * ( total / m_NbBin ) ) );
  // Do i need to static_cast in a an assignation?
  // Warning!!!! Need to handle out of bound int!!!

  for( unsigned int i = 0 ; i < m_NbBin ; i++ )
    {
    if ( static_cast<unsigned int>( oit.Get()[i] ) > height )
      {
      rest += oit.Get()[i] - height ;
      oit.Get()[i] = height ;
      }
    }
  height = rest / m_NbBin;
  rest = rest % m_NbBin;
  for( unsigned int i = 0 ; i < m_NbBin ; i++ )
    {
    oit.Get()[i] += height ;
    if ( i > (m_NbBin - rest)/2 && i <= (m_NbBin - rest)/2 + rest )
      {
      ++oit.Get()[i];
      }
    }
}

template <class TInputImage, class TOutputImage >
typename TOutputImage::Pointer ComputeHistoFilter < TInputImage , TOutputImage >
::GetHistoOutput()
{
  assert( this->itk::ProcessObject::GetOutput( 1 ) );

  return dynamic_cast< TOutputImage * >(
           this->itk::ProcessObject::GetOutput(1) );
}

template <class TInputImage, class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::SetRequestedRegion( itk::ImageBase<2> * image )
{
  OutputImageRegionType histoRegion ( 
                      GetHistoOutput()->GetRequestedRegion() );

  IndexType start ;
  start[0] = histoRegion.GetIndex()[0] * m_ThumbSize[0];
  start[1] = histoRegion.GetIndex()[1] * m_ThumbSize[1];

  SizeType size ;
  size[0] = histoRegion.GetSize()[0] * m_ThumbSize[0];
  size[1] = histoRegion.GetSize()[1] * m_ThumbSize[1];

  typename OutputImageType::RegionType outputRequestedRegion;
  outputRequestedRegion.SetIndex( start );
  outputRequestedRegion.SetSize( size );

  outputRequestedRegion.Crop( image->GetLargestPossibleRegion() );
  image->SetRequestedRegion( outputRequestedRegion );
}

template <class TInputImage , class TOutputImage >
void ComputeHistoFilter < TInputImage , TOutputImage >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Is no data activated: " << m_NoDataFlag << std::endl;
  os << indent << "No Data: " << m_NoData << std::endl;
  os << indent << "Minimum: " << m_Min << std::endl;
  os << indent << "Maximum: " << m_Max << std::endl;
  os << indent << "Step: " << m_Step << std::endl;
  os << indent << "Number of bin: " << m_NbBin << std::endl;
  os << indent << "Thumbnail size: " << m_ThumbSize << std::endl;
  os << indent << "Threshold value: " << m_Threshold << std::endl;
}
  
} // End namespace otb

#endif
