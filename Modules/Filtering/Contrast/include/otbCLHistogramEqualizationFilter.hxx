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

#ifndef otbCLHistogramEqualizationFilter_hxx
#define otbCLHistogramEqualizationFilter_hxx

#include "otbCLHistogramEqualizationFilter.h"
#include "itkImageRegionIterator.h"

#include <limits>

namespace otb
{
template < class TInputImage , class TOutputImage >
CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::CLHistogramEqualizationFilter():
m_HistoFilter( HistoFilter::New() ) ,
m_GainLutFilter ( GainLutFilter::New() ) ,
m_ApplyGainFilter ( ApplyGainFilter::New() ) ,
m_StreamingImageFilter ( StreamingImageFilter::New() ) ,
m_BufferFilter ( BufferFilter::New() )
{
  m_Min = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_Max = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_NbBin = 256;
  m_Threshold = -1;
  m_NoDataFlag = false;
  m_NoData = std::numeric_limits< InputPixelType >::quiet_NaN();
  m_ThumbSize.Fill(0);
  m_GainLutFilter->SetInput( m_HistoFilter->GetHistoOutput() );
  m_StreamingImageFilter->SetInput( m_GainLutFilter->GetOutput() );
  m_ApplyGainFilter->SetInputLut( m_StreamingImageFilter->GetOutput() );
  m_ApplyGainFilter->SetInputImage( m_BufferFilter->GetOutput() );
}

template < class TInputImage , class TOutputImage >
void CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::UpdateOutputInformation()
{ 
  const InputImageType * input =  this->GetInput() ;
  m_HistoFilter->SetInput( input );
  m_BufferFilter->SetInput( input );
  m_ApplyGainFilter->GetOutput()->UpdateOutputInformation();
  this->GetOutput()->CopyInformation( m_ApplyGainFilter->GetOutput() );
}

template < class TInputImage , class TOutputImage >
void CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::PropagateRequestedRegion( itk::DataObject * output )
{ 
  m_ApplyGainFilter->GetOutput()->SetRequestedRegion( static_cast<OutputImageType *>(output)->GetRequestedRegion() );
  m_ApplyGainFilter->GetOutput()->PropagateRequestedRegion();
}

template < class TInputImage , class TOutputImage >
void CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::GenerateData()
{
  m_ApplyGainFilter->GraftOutput( this->GetOutput() );
  m_ApplyGainFilter->Update();
  this->GraftOutput( m_ApplyGainFilter->GetOutput() );
  
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage , class TOutputImage >
void CLHistogramEqualizationFilter < TInputImage , TOutputImage >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Minimum : " << m_Min << std::endl;
  os << indent << "Maximum : " << m_Max << std::endl;
  os << indent << "Bin Number : " << m_NbBin << std::endl;
  os << indent << "Thumbnail size : " << m_ThumbSize << std::endl;
  os << indent << "Threshold value : " << m_Threshold << std::endl;
  os << indent << "Is no data activated : " << m_NoDataFlag << std::endl;
  os << indent << "No Data : " << m_NoData << std::endl;
}

  
}  // End namespace otb

#endif
