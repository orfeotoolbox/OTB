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

#ifndef otbComputeGainFilter_txx
#define otbComputeGainFilter_txx

#include "otbComputeGainFilter.h"
#include "itkImageRegionIterator.h"

#include <limits>

// #define DEBUG
namespace otb
{
template <class TInputImage , class TLut , class TOutputImage >
ComputeGainFilter < TInputImage , TLut , TOutputImage >
::ComputeGainFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_Min = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_Max = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NoData = std::numeric_limits< InputPixelType >::quiet_NaN();
}

template <class TInputImage , class TLut , class TOutputImage >
void ComputeGainFilter < TInputImage , TLut , TOutputImage >
::SetInputImage(const InputImageType * input)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput( 0 , const_cast<InputImageType *>( input ) );
}

template <class TInputImage , class TLut , class TOutputImage >
const TInputImage * ComputeGainFilter < TInputImage , TLut , TOutputImage >
::GetInputImage() const
{
  return static_cast< const InputImageType * >
        ( this->itk::ProcessObject::GetInput(0) );
}

template <class TInputImage , class TLut , class TOutputImage >
void ComputeGainFilter < TInputImage , TLut , TOutputImage >
::SetInputLut(const LutType * lut)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput( 1 , const_cast<LutType *>( lut ) );
}

template <class TInputImage , class TLut , class TOutputImage >
const TLut * ComputeGainFilter < TInputImage , TLut , TOutputImage >
::GetInputLut() const
{
  return static_cast< const LutType * >
       ( this->itk::ProcessObject::GetInput(1) );
}

template <class TInputImage , class TLut , class TOutputImage >
void ComputeGainFilter < TInputImage , TLut , TOutputImage >
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  typename InputImageType::Pointer input = const_cast<InputImageType *>( GetInputImage() );
  typename LutType::Pointer lut = const_cast<LutType *>( GetInputLut() );

  typename OutputImageType::Pointer output =( this->GetOutput() );

  input->SetRequestedRegion( output->GetRequestedRegion() );
  lut->SetRequestedRegion( lut->GetLargestPossibleRegion() );

}


template <class TInputImage , class TLut , class TOutputImage >
void ComputeGainFilter < TInputImage , TLut , TOutputImage >
::BeforeThreadedGenerateData()
{
  typename InputImageType::ConstPointer input = GetInputImage();
  typename LutType::ConstPointer lut = GetInputLut();

  m_LutSize = lut->GetLargestPossibleRegion().GetSize();
  m_ThumbSize[0] = input->GetLargestPossibleRegion().GetSize()[0] / m_LutSize[0];
  m_ThumbSize[1] = input->GetLargestPossibleRegion().GetSize()[1] / m_LutSize[1];

  m_Step = static_cast<double>( m_Max - m_Min ) \
                / static_cast<double>( lut->GetVectorLength() -1 );
}

template <class TInputImage , class TLut , class TOutputImage >
void ComputeGainFilter < TInputImage , TLut , TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread ,
                             ThreadIdType itkNotUsed(threadId) )
{
  typename InputImageType::ConstPointer input = GetInputImage();
  typename LutType::ConstPointer lut = GetInputLut();
  typename OutputImageType::Pointer output = this->GetOutput();

  typename InputImageType::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread ,
                                          outputRegionForThread);
  // Is it usefull???

  // support progress methods/callbacks
  // itk::ProgressReporter progress(this , threadId , 
  //               outputRegionForThread.GetNumberOfPixels() );

  itk::ImageRegionConstIterator < InputImageType > it ( input , 
                                                        inputRegionForThread );

  itk::ImageRegionIterator <OutputImageType > oit ( output ,
                                                    outputRegionForThread );
  it.GoToBegin();
  oit.GoToBegin();
  int pixelLutValue = 0;
  float gain = 0.0;
  while ( !oit.IsAtEnd() )
    {
    if( it.Get() == m_NoData || it.Get() > m_Max || it.Get() < m_Min )
      {
      oit.Set( static_cast<OutputPixelType>( it.Get() ) );
      ++it;
      ++oit;
      continue;
      }
    pixelLutValue = static_cast<int>( std::round( ( it.Get() - m_Min ) / m_Step ) );
    gain = InterpoleGain( lut , pixelLutValue , it.GetIndex() );
    oit.Set( static_cast<OutputPixelType>( gain * it.Get() ) );
    ++it;
    ++oit;
    }

}

template <class TInputImage , class TLut , class TOutputImage >
float ComputeGainFilter < TInputImage , TLut , TOutputImage >
::InterpoleGain( typename LutType::ConstPointer gridLut,
                 int pixelLutValue ,
                 typename InputImageType::IndexType index)
{
  LutIndexType lutIndex;
  lutIndex[0] = index[0]/m_ThumbSize[0];
  lutIndex[1] = index[1]/m_ThumbSize[1];
  float x = static_cast< float >(index[0]%m_ThumbSize[0]) \
            / static_cast< float >(m_ThumbSize[0]);
  float y = static_cast< float >(index[1]%m_ThumbSize[1]) \
            / static_cast< float >(m_ThumbSize[1]);
  float disty = std::abs(y - 0.5);
  float distx = std::abs(x - 0.5);
  float w = (1 - distx )*(1 - disty );
  float gain = gridLut->GetPixel(lutIndex)[pixelLutValue] * \
              (1 - distx ) * (1 - disty ) ;
  typename LutType::OffsetType rightOffSet , upOffSet , leftOffSet , downOffSet;
  rightOffSet.Fill(0);
  rightOffSet[0] = 1 ;
  bool right = x>=0.5 && 
        ( ( rightOffSet[0] + lutIndex[0] ) < static_cast<int>( m_LutSize[0] ) );
  leftOffSet.Fill(0);
  leftOffSet[0] = -1 ;
  bool left = x<=0.5 && 
              ( ( leftOffSet[0] + lutIndex[0] ) >= 0 );
  upOffSet.Fill(0);
  upOffSet[1] = -1 ;
  bool up = y<=0.5 && 
              ( ( upOffSet[1] + lutIndex[1] ) >= 0 );
  downOffSet.Fill(0);
  downOffSet[1] = 1 ;
  bool down = y>=0.5 && 
        ( ( downOffSet[1] + lutIndex[1] ) < static_cast<int>( m_LutSize[1] ) );
  if ( right )
    {
    gain += ( gridLut->GetPixel(lutIndex + rightOffSet)[pixelLutValue] ) \
            * (1 - disty ) * distx;
    w += (1 - disty ) * distx;
    }
  if ( left )
    {
    gain += ( gridLut->GetPixel(lutIndex + leftOffSet)[pixelLutValue] ) \
            * (1 - disty ) * distx;
    w += (1 - disty ) * distx;
    }
  if ( up )
    {
    gain += ( gridLut->GetPixel(lutIndex + upOffSet)[pixelLutValue] ) \
            * disty * (1 - distx );
    w += disty * (1 - distx );
    }
  if ( down )
    {
    gain += ( gridLut->GetPixel(lutIndex + downOffSet)[pixelLutValue] ) \
            * disty * (1 - distx );
    w += disty * (1 - distx );
    }
  if ( up && left )
    {
    gain += gridLut->GetPixel(lutIndex + upOffSet + leftOffSet)[pixelLutValue] \
              * disty * distx;
    w += disty * distx;
    }
  if ( down && left )
    {
    gain += gridLut->GetPixel(lutIndex + downOffSet + leftOffSet)[pixelLutValue] \
              * disty * distx;
    w += disty * distx;
    }
  if ( up && right )
    {
    gain += gridLut->GetPixel(lutIndex + upOffSet + rightOffSet)[pixelLutValue] \
              * disty * distx;
    w += disty * distx ;
    }
  if ( down && right )
    {
    gain += gridLut->GetPixel(lutIndex + downOffSet + rightOffSet)[pixelLutValue] \
              * disty * distx;
    w += disty * distx;
    }

  return gain/w;
}

  
}  // End namespace otb

#endif