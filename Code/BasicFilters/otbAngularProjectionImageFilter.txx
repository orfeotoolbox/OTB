/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbAngularProjectionImageFilter_txx
#define __otbAngularProjectionImageFilter_txx
#include "otbAngularProjectionImageFilter.h"

#include <vnl/vnl_math.h>

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include <itkProgressAccumulator.h>

namespace otb {

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::AngularProjectionImageFilter ()
{
  this->SetNumberOfRequiredInputs(NumberOfInputImages);
  this->SetNumberOfOutputs(1);
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
  if ( i < this->GetNumberOfInputs() )
  {
    return 0;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(i) );
}

template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
void
AngularProjectionImageFilter< TInputImage, TOutputImage, TAngleArray, TPrecision >
::ThreadedGenerateData
  ( const OutputImageRegionType & outputRegionForThread, int threadId )
{
  itk::ProgressReporter reporter(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() );

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  bool iteratorsAtEnd = false;
  ImageRegionConstIteratorVectorType it ( NumberOfInputImages );
  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
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
      outIter.Set( GenerateData( it ) );

    ++outIter;
    for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
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
::GenerateData ( const ImageRegionConstIteratorVectorType & it ) const
{
  PrecisionType output = 0;

  if ( NumberOfInputImages == 2 )
  {
    PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[0] );
    output = static_cast<PrecisionType>( it[0].Get() ) * vcl_cos( alpha ) 
              - static_cast<PrecisionType>( it[1].Get() ) * vcl_sin( alpha );
  }
  else if ( NumberOfInputImages == 3 )
  {
    PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[0] );
    PrecisionType beta = static_cast<PrecisionType>( m_AngleArray[1] );

    output = static_cast<PrecisionType>( it[0].Get() ) * vcl_cos( alpha )
            - static_cast<PrecisionType>( it[1].Get() ) * vcl_sin( alpha ) * vcl_cos ( beta )
            + static_cast<PrecisionType>( it[2].Get() ) * vcl_sin( alpha ) * vcl_sin ( beta );
  }
  else
  {
    unsigned int i = NumberOfInputImages-1;
    output = static_cast<PrecisionType>( it[i--].Get() );

    do {
      PrecisionType alpha = static_cast<PrecisionType>( m_AngleArray[i] );
      output = static_cast<PrecisionType>( it[i].Get() ) * vcl_cos( alpha )
                - output * vcl_sin( alpha );
    } while ( i-- == 0 );
  }
  
  return static_cast<OutputImagePixelType>( output );
}

} // end of namespace otb

#endif


