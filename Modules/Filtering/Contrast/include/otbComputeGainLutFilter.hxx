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

#ifndef otbComputeGainLutFilter_hxx
#define otbComputeGainLutFilter_hxx

#include "otbComputeGainLutFilter.h"
#include "itkImageRegionIterator.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT
#include <limits>
#include <numeric>

namespace otb
{

template < class TInputImage, class TOutputImage >
ComputeGainLutFilter < TInputImage , TOutputImage >
::ComputeGainLutFilter()
{
  OTB_DISABLE_DYNAMIC_MT
    m_NbBin = 256;
  m_NbPixel = 0;
  m_Min = std::numeric_limits< double >::quiet_NaN();
  m_Max = std::numeric_limits< double >::quiet_NaN();
  m_Step = -1;
}

template <class TInputImage , class TOutputImage >
void ComputeGainLutFilter <TInputImage , TOutputImage >
::BeforeThreadedGenerateData()
{
  m_NbBin = this->GetInput()->GetNumberOfComponentsPerPixel();
  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( m_NbBin -1 );
}

template <class TInputImage , class TOutputImage >
void ComputeGainLutFilter <TInputImage , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       itk::ThreadIdType itkNotUsed(threadId) )
{
  assert( m_Step > 0 );
  assert( m_NbBin > 0 );
  // TODO error
  // itk::ProgressReporter progress(this , threadId , 
  //               outputRegionForThread.GetNumberOfPixels() );

  typename InputImageType::ConstPointer input ( this->GetInput() );
  typename OutputImageType::Pointer output ( this->GetOutput() );

  itk::ImageRegionConstIterator < InputImageType > it ( input , 
                                                        outputRegionForThread );

  itk::ImageRegionIterator <OutputImageType > oit ( output ,
                                                    outputRegionForThread );

  HistoType target;
  target.SetSize( m_NbBin );

  HistoType currentHisto;

  LutType lut;
  lut.SetSize( m_NbBin );

  for (it.GoToBegin() , oit.GoToBegin() ; !oit.IsAtEnd() || !it.IsAtEnd() ;
       ++oit , ++it )
    {
    currentHisto = it.Get();
    target.Fill(0);
    lut.Fill(-1);
    if ( IsValid( currentHisto ) )
      {
      CreateTarget( currentHisto , target );
      Equalized( currentHisto , target , lut ); 
      }
    oit.Set( lut );
    }
  assert ( oit.IsAtEnd() && it.IsAtEnd() );
}

template <class TInputImage, class TOutputImage >
typename TOutputImage::InternalPixelType 
  ComputeGainLutFilter < TInputImage , TOutputImage >
::PostProcess( unsigned int countValue ,
               unsigned int countMapValue )
{ 
  double denum ( countValue * m_Step + m_Min );
  if ( denum == 0 )
    return 0;
  return static_cast< OutputPixelType > ( (countMapValue * m_Step + m_Min)
          / denum );
}

template <class TInputImage, class TOutputImage >
void ComputeGainLutFilter < TInputImage , TOutputImage >
::Equalized( const HistoType & inputHisto ,
             HistoType & targetHisto ,
             LutType & lut)
{
  unsigned int countValue(0) , countMapValue(0) ;
  lut[countValue] = 1; // Black stays black
  ++countValue;
  unsigned int countInput ( inputHisto[ 0 ] + inputHisto[ countValue ] );
  lut[m_NbBin - 1 ] = 1 ; // White stays white
  unsigned int countTarget ( targetHisto[ countMapValue ] );

  while ( ( countMapValue <  m_NbBin ) && countValue < ( m_NbBin - 1 ) )
    {
    if ( countInput > countTarget )
      {
      ++countMapValue;
      countTarget += targetHisto[ countMapValue ];
      }
    else
      { 
      lut[countValue] =  PostProcess( countValue , countMapValue );
      ++countValue;
      countInput  += inputHisto[ countValue ];
      }
    }
  for (unsigned int i = 0 ; i < m_NbBin ; i++)
    {
    if (lut[i] == -1)
      {
      lut[i] = 1;
      } 
    }
}

template <class TInputImage, class TOutputImage >
void ComputeGainLutFilter < TInputImage , TOutputImage >
::CreateTarget( const HistoType & inputHisto ,
                HistoType & targetHisto )
{
  unsigned int nbPixel(0);
  for ( unsigned int i = 0 ; i < m_NbBin ; i++ )
    {
    nbPixel += inputHisto[i];
    }
  unsigned int rest ( nbPixel % m_NbBin ) , height ( nbPixel / m_NbBin );
  targetHisto.Fill(height);
  for ( unsigned int i = 0 ; i < rest ; i++ )
    {
    ++targetHisto[ ( m_NbBin - rest ) / 2 + i ];
    }  
}

template <class TInputImage, class TOutputImage >
bool ComputeGainLutFilter < TInputImage , TOutputImage >
::IsValid( const HistoType & inputHisto )
{
  long acc = std::accumulate( &inputHisto[0] , 
                              &inputHisto[ m_NbBin - 1 ] ,
                              0);
  return acc >= (0.5 * m_NbPixel);
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage , class TOutputImage >
void ComputeGainLutFilter < TInputImage , TOutputImage >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Minimum: " << m_Min << std::endl;
  os << indent << "Maximum: " << m_Max << std::endl;
  os << indent << "Step: " << m_Step << std::endl;
  os << indent << "Number of bin: " << m_NbBin << std::endl;
  os << indent << "Number of pixel by histogram: " << m_NbPixel << std::endl;
}

} // End namespace otb

#endif
