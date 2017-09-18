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

#ifndef otbComputeGainLutFilter_txx
#define otbComputeGainLutFilter_txx

#include "otbComputeGainLutFilter.h"
#include "itkImageRegionIterator.h"

#include <limits>


namespace otb
{

template < class TInputImage, class TOutputImage >
ComputeGainLutFilter < TInputImage , TOutputImage >
::ComputeGainLutFilter()
{
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
  assert(m_Step>0);
  // itk::ProgressReporter progress(this , threadId , 
  //               outputRegionForThread.GetNumberOfPixels() );

  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = this->GetOutput();

  typename InputImageType::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread , outputRegionForThread);
  // Is it usefull???

  itk::ImageRegionConstIterator < InputImageType > it ( input , 
                                                        inputRegionForThread );

  itk::ImageRegionIterator <OutputImageType > oit ( output ,
                                                    outputRegionForThread );
  HistoType target;
  target.SetSize( m_NbBin );
  LutType lut;
  lut.SetSize( m_NbBin );
  it.GoToBegin();
  oit.GoToBegin();
  while ( !oit.IsAtEnd() )
    {
      target.Fill(0);
      lut.Fill(-1);
      if ( IsValide( it.Get() ) )
        {
        CreateTarget( it.Get() , target );
        Equalized( it.Get() , target , lut ); 
        }
      oit.Set(lut);
      ++oit;
      ++it;
    }
}

template <class TInputImage, class TOutputImage >
typename TOutputImage::InternalPixelType ComputeGainLutFilter < TInputImage , TOutputImage >
::PostProcess( int countValue ,
               int countMapValue )
{ 
  float denum = countValue * m_Step + m_Min;
  if ( denum == 0 )
    return 0;
  return static_cast< OutputPixelType > ((countMapValue * m_Step + m_Min) \
          / denum );
}

template <class TInputImage, class TOutputImage >
void ComputeGainLutFilter < TInputImage , TOutputImage >
::Equalized( const HistoType & inputHisto ,
             HistoType & targetHisto ,
             LutType & lut)
{
  int countMapValue = 0;
  int countValue = 0;
  lut[countValue] = 1; // Black stays black
  ++countValue;
  int countInput  = inputHisto[ 0 ] + inputHisto[ countValue ];
  lut[m_NbBin - 1 ] = 1 ; // White stays white
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
      lut[countValue] =  PostProcess( countValue ,countMapValue );
      ++countValue;
      countInput  += inputHisto[ countValue ];
      }
    }
  for (int i = 0 ; i < m_NbBin ; i++)
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

template <class TInputImage, class TOutputImage >
bool ComputeGainLutFilter < TInputImage , TOutputImage >
::IsValide( const HistoType & inputHisto )
{
  long acc(0);
  for ( int i = 0 ; i < m_NbBin ; i++ )
  {
    acc+= inputHisto[i] ;
  }
  if ( acc < 0.5*m_NbPixel )
    return false;
  return true;
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