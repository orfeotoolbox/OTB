/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbStationaryFilterBank_txx
#define __otbStationaryFilterBank_txx

#include "otbStationaryFilterBank.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"

namespace otb {

template < class TInputImage, class TOutputImage, class TLowPassOperator, class THighPassOperator >
StationaryFilterBank< TInputImage, TOutputImage, TLowPassOperator, THighPassOperator >
::StationaryFilterBank ()
{
  this->SetNumberOfRequiredInputs(1);

  unsigned int numOfOutput = 1<<InputImageType::ImageDimension;

	this->SetNumberOfOutputs( numOfOutput );
  for ( unsigned i = 1; i < numOfOutput; i++ )
  {
    this->SetNthOutput(i,OutputImageType::New());
  }

  m_UpSampleFactor = 0;
}

template < class TInputImage, class TOutputImage, class TLowPassOperator, class THighPassOperator >
void
StationaryFilterBank< TInputImage, TOutputImage, TLowPassOperator, THighPassOperator >
::ThreadedGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfOutputs() );

  ThreadedGenerateData( reporter, outputRegionForThread, threadId );
}

template < class TInputImage, class TOutputImage, class TLowPassOperator, class THighPassOperator >
void
StationaryFilterBank< TInputImage, TOutputImage, TLowPassOperator, THighPassOperator >
::ThreadedGenerateData 
( itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  const InputImageType * input = this->GetInput();

  unsigned int idx = InputImageType::ImageDimension-1;

  OutputImageType * outputLowPass = this->GetOutput( 0 );
  OutputImageType * outputHighPass = this->GetOutput( 1<<idx );

  if ( outputLowPass == 0 )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__, "Sortie 0 nulle", ITK_LOCATION );
  }

  if ( outputHighPass == 0 )
  {
    itk::OStringStream msg;
		msg << "Sortie 1<<" << idx << " = " << (1<<idx) << " nulle\n";
    msg << "Nombre de sortie attendue " << this->GetNumberOfOutputs() << "\n";
    throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );
  }

  typedef itk::ConstNeighborhoodIterator< InputImageType > InputNeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< InputImageType > InputInnerProductType;
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > InputFaceCalculatorType;
  typedef typename InputFaceCalculatorType::FaceListType InputFaceListType;
  typedef typename InputFaceListType::iterator InputFaceListIterator;

  InputInnerProductType innerProduct;
  InputFaceCalculatorType faceCalculator;  
  InputFaceListType faceList;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(idx);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFactor() );
  highPassOperator.CreateDirectional();

  faceList = faceCalculator( input, outputRegionForThread, highPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    InputNeighborhoodIteratorType it ( highPassOperator.GetRadius(), input, *faceIt );
    IteratorType out ( outputHighPass, *faceIt );

    for ( it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out )  
    {  
      out.Set( innerProduct( it, highPassOperator ) );
      reporter.CompletedPixel();
    }
  }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(idx);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFactor() );
  lowPassOperator.CreateDirectional();

  faceList = faceCalculator( input, outputRegionForThread, lowPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    InputNeighborhoodIteratorType it ( lowPassOperator.GetRadius(), input, *faceIt );
    IteratorType out ( outputLowPass, *faceIt );

    for ( it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out )  
    {  
      out.Set( innerProduct( it, lowPassOperator ) );
      reporter.CompletedPixel();
    }
  }

  if ( idx > 0 )
  {
    ThreadedGenerateData( 0, idx-1, reporter, outputRegionForThread, threadId );
    ThreadedGenerateData( 1<<idx, idx-1, reporter, outputRegionForThread, threadId );
  }
}

template < class TInputImage, class TOutputImage, class TLowPassOperator, class THighPassOperator >
void
StationaryFilterBank< TInputImage, TOutputImage, TLowPassOperator, THighPassOperator >
::ThreadedGenerateData 
( unsigned int idx, unsigned int direction, itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageType * input = this->GetOutput( idx );
  OutputImageType * outputHighPass = this->GetOutput( idx + (1<<direction) );

  OutputImagePointerType outputLowPass = OutputImageType::New();
  outputLowPass->SetRegions( outputRegionForThread );
  outputLowPass->Allocate();

  InnerProductType innerProduct;
  FaceCalculatorType faceCalculator;  
  FaceListType faceList;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(direction);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFactor() );
  highPassOperator.CreateDirectional();

  faceList = faceCalculator( input, outputRegionForThread, highPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    NeighborhoodIteratorType it ( highPassOperator.GetRadius(), input, *faceIt );
    IteratorType out ( outputHighPass, *faceIt );

    for ( it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out )  
    {  
      out.Set( innerProduct( it, highPassOperator ) );
      reporter.CompletedPixel();
    }
  }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(direction);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFactor() );
  lowPassOperator.CreateDirectional();

  faceList = faceCalculator( input, outputRegionForThread, lowPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    NeighborhoodIteratorType it ( lowPassOperator.GetRadius(), input, *faceIt );
    IteratorType out ( outputLowPass, *faceIt );

    for ( it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out )  
    {  
      out.Set( innerProduct( it, lowPassOperator ) );
      reporter.CompletedPixel();
    }
  }

  // Swap input and lowPassOutput
  itk::ImageRegionConstIterator< OutputImageType > inIt ( outputLowPass, outputRegionForThread );
  IteratorType outIt ( this->GetOutput( idx ), outputRegionForThread );

  for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt )
  {
    outIt.Set( inIt.Get() );
  }

  if ( direction != 0 )
  {
    ThreadedGenerateData( idx, direction-1, reporter, outputRegionForThread, threadId );
    ThreadedGenerateData( idx + (1<<direction), direction-1, reporter, outputRegionForThread, threadId );
  }
}

} // end of namespace

#endif




