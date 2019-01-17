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

#ifndef otbAngularProjectionImageFilter_hxx
#define otbAngularProjectionImageFilter_hxx
#include "otbAngularProjectionImageFilter.h"

#include <vnl/vnl_math.h>

#include <itkImageRegionIterator.h>
#include <itkProgressAccumulator.h>

namespace otb {

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::AngularProjectionImageFilter ()
{
  //this->SetNumberOfRequiredInputs(NumberOfInputImages);
  this->SetNumberOfRequiredOutputs(1);
}

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
void
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::SetInput ( unsigned int i, const InputImageType * img )
{
  this->itk::ProcessObject::SetNthInput(i,
    const_cast< InputImageType * >( img ) );
}

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
const TInputImage *
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::GetInput ( unsigned int i ) const
{
  if ( i >= this->GetNumberOfInputs() )
  {
    return nullptr;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(i) );
}

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
void
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::ThreadedGenerateData
  ( const OutputImageRegionType & outputRegionForThread, itk::ThreadIdType threadId )
{
  itk::ProgressReporter reporter(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() );

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  bool iteratorsAtEnd = false;
  ImageRegionConstIteratorVectorType it ( this->GetNumberOfInputs() );
  for ( unsigned int i = 0; i < this->GetNumberOfInputs(); ++i )
  {
    it[i] = ImageRegionConstIteratorType( this->GetInput(i), inputRegionForThread );
    it[i].GoToBegin();
    if ( it[i].IsAtEnd() )
      iteratorsAtEnd = true;
  }

  itk::ImageRegionIterator<OutputImageType> outIter
    ( this->GetOutput(), outputRegionForThread );
  outIter.GoToBegin();

  while ( !iteratorsAtEnd && !outIter.IsAtEnd() )
  {
    outIter.Set( InternalGenerateData( it ) );

    ++outIter;
    for ( unsigned int i = 0; i < this->GetNumberOfInputs(); ++i )
    {
      ++(it[i]);
      if ( it[i].IsAtEnd() )
        iteratorsAtEnd = true;
    }

    reporter.CompletedPixel();
  }
}

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
typename AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::OutputImagePixelType
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::InternalGenerateData ( const ImageRegionConstIteratorVectorType & it ) const
{
  PrecisionType output = 0;

  if ( this->GetNumberOfInputs() == 2 )
  {
    PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[0] );
    output = static_cast<PrecisionType>( it[0].Get() ) * std::cos( alpha )
              - static_cast<PrecisionType>( it[1].Get() ) * std::sin( alpha );
  }
  else if ( this->GetNumberOfInputs() == 3 )
  {
    PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[0] );
    PrecisionType beta = static_cast<PrecisionType>( m_AngleArray[1] );

    output = static_cast<PrecisionType>( it[0].Get() ) * std::cos( alpha )
            - static_cast<PrecisionType>( it[1].Get() ) * std::sin( alpha ) * std::cos ( beta )
            + static_cast<PrecisionType>( it[2].Get() ) * std::sin( alpha ) * std::sin ( beta );
  }
  else
  {
    unsigned int i = this->GetNumberOfInputs()-1;
    output = static_cast<PrecisionType>( it[i--].Get() );

    do {
      PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[i] );
      output = static_cast<PrecisionType>( it[i].Get() ) * std::cos( alpha )
                - output * std::sin( alpha );
    } while ( i-- == 0 );
  }

  return static_cast<OutputImagePixelType>( output );
}

} // end of namespace otb

#endif


