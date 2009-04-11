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

#include "otbMacro.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"

namespace otb {

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::StationaryFilterBank ()
{
  switch ( DirectionOfTransformation )
  {
    case FORWARD:
    {
      this->SetNumberOfRequiredInputs(1);

      unsigned int numOfOutputs = 1<<InputImageType::ImageDimension;

      this->SetNumberOfOutputs( numOfOutputs );
      for ( unsigned i = 1; i < numOfOutputs; i++ )
      {
        this->SetNthOutput(i,OutputImageType::New());
      }
      
      break;
    }
    case INVERSE:
    {
      this->SetNumberOfRequiredOutputs(1);
      this->SetNumberOfInputs( 1 << InputImageType::ImageDimension );
      break;
    }
    default:
    {
      itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
      break;
    }
  }
  m_UpSampleFilterFactor = 0;
  m_SubSampleImageFactor = 1;
}


template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  switch ( DirectionOfTransformation )
  {
    case FORWARD:
    {
      /**
       * If necessary, we do not say to the filter, at this
       * step, that we will down sample the output images...
       */
      break;
    }
    case INVERSE:
    {
      otbGenericMsgDebugMacro( << " down sampling output regions by a factor of " << GetSubSampleImageFactor() );

      this->GetOutput()->CopyInformation( this->GetInput() );
      this->GetOutput()->SetRegions( this->GetInput()->GetLargestPossibleRegion() );

      if ( GetSubSampleImageFactor() > 1 )
      {
        for ( unsigned int i = 0; i < ImageDimension; i++ )
        {
          this->GetOutput()->GetRegion().GetIndex()[i] /= GetSubSampleImageFactor();
          this->GetOutput()->GetRegion().GetSize()[i] /= GetSubSampleImageFactor();
        }
      }
      break;
    }
    default:
    {
      itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
      break;
    }
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::BeforeThreadedGenerateData ()
{
  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    otbGenericMsgDebugMacro(<<"Subsample inputs by a factor of " << GetSubSampleImageFactor() );

    // Sub sample inputs
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::AfterThreadedGenerateData ()
{
  if ( static_cast<int>( DirectionOfTransformation ) == FORWARD )
  {
    otbGenericMsgDebugMacro(<<"Downsample outputs by a factor of " << GetSubSampleImageFactor() );

    // Down sample inputs
  }
}


template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  switch ( DirectionOfTransformation )
  {
    case FORWARD:
      return ThreadedForwardGenerateData( outputRegionForThread, threadId );
      break;
    case INVERSE:
      return ThreadedInverseGenerateData( outputRegionForThread, threadId );
      break;
    default:
      itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
      break;
  } 
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedForwardGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId )
{

  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfOutputs() );

  ThreadedForwardGenerateData( reporter, outputRegionForThread, threadId );
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedForwardGenerateData 
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
    msg << "Output number 1<<" << idx << " = " << (1<<idx) << " null\n";
    msg << "Number of excpected outputs " << this->GetNumberOfOutputs() << "\n";
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
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
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
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
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
    ThreadedForwardGenerateData( 0, idx-1, reporter, outputRegionForThread, threadId );
    ThreadedForwardGenerateData( 1<<idx, idx-1, reporter, outputRegionForThread, threadId );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedForwardGenerateData 
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
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
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
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
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
    outIt.Set( static_cast<InputPixelType>( inIt.Get() ) );
  }

  if ( direction != 0 )
  {
    ThreadedGenerateData( idx, direction-1, reporter, outputRegionForThread, threadId );
    ThreadedGenerateData( idx + (1<<direction), direction-1, reporter, outputRegionForThread, threadId );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedInverseGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfInputs() );

  ThreadedInverseGenerateData( reporter, outputRegionForThread, threadId );
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedInverseGenerateData 
( itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  InputImageRegionType inputRegionForThread;
	this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );
  
  unsigned int idx = InputImageType::ImageDimension-1;

  InputImagePointerType imgLowPassPointer = InputImageType::New();
  InputImageType * imgLowPass = this->GetInput(0);
  if ( idx != 0 )
  {
    imgLowPassPointer->SetRegions( inputRegionForThread );
    imgLowPassPointer->Allocate();

    ThreadedInverseGenerateData( 0, idx-1, imgLowPassPointer, reporter, inputRegionForThread, threadId );

    imgLowPass = imgLowPassPointer.GetPointer();
  }

  InputImagePointerType imgHighPassPointer = InputImageType::New();
  InputImageType * imgHighPass = this->GetInput(1);
  if ( idx != 0 )
  {
    imgHighPassPointer->SetRegions( inputRegionForThread );
    imgHighPassPointer->Allocate();

    ThreadedInverseGenerateData( 1<<idx, idx-1, imgHighPassPointer, reporter, inputRegionForThread, threadId );
  
    imgHighPass = imgHighPassPointer.GetPointer();
  }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(idx);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(idx);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  // Faces iterations
  typedef NeighborhoodIteratorType::RadiusType radiusMax;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
  {
    radiusMax[i] = lowPassOperator.GetRadius()[i];
    if ( radius[i] < highPassOperator.GetRadius()[i] )
      radius[i] = highPassOperator.GetRadius()[i];
  }

  FaceCalculatorType faceCalculator;  
  FaceListType faceList;
  faceList = faceCalculator( imgHighPass, inputRegionForThread, radiusMax );

  InnerProductType innerProduct;

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    itk::ImageRegionIterator< OutputImageType > out ( this->GetOutput(), *faceIt );

    NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), imgLowPass, *faceIt );
    NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), imgHighPass, *faceIt );

    lowIter.GoToBegin();
    highIter.GoToBegin();
    out.GoToBegin();

    while ( !out.IsAtEnd() )
    {  
      out.Set( static_cast< OutputPixelType >( 
                innerProduct( lowIter, lowPassOperator ) 
                  + innerProduct( highIter, highPassOperator ) ) );

      ++lowIter;
      ++highIter;
      ++out;

      reporter.CompletedPixel();
    }
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
StationaryFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::ThreadedInverseGenerateData 
( unsigned int idx, unsigned int direction, 
  InputImagePointerType & outputImage,
  itk::ProgressReporter & reporter,
  const InputImageRegionType& inputRegionForThread, int threadId )
{
  InputImagePointerType imgLowPassPointer = InputImageType::New();
  InputImageType * imgLowPass = this->GetInput(idx);
  if ( direction != 0 )
  {
    imgLowPassPointer->SetRegions( inputRegionForThread );
    imgLowPassPointer->Allocate();

    ThreadedGenerateData( idx, direction-1, imgLowPassPointer, reporter, outputRegionForThread, threadId );

    imgLowPass = imgLowPassPointer.GetPointer();
  }

  InputImagePointerType imgHighPassPointer = InputImageType::New();
  InputImageType * imgHighPass = this->GetInput(idx + (1<<direction));
  if ( direction != 0 )
  {
    imgHighPassPointer->SetReions( inputRegionForThread );
    imgHighPassPointer->Allocate();

    ThreadedInverseGenerateData( idx + (1<<direction), direction-1, imgHighPassPointer, reporter, inputRegionForThread, threadId );

    imgHighPass = imgHighPassPointer.GetPointer();
  }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(idx);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(idx);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  // Faces iterations
  typedef NeighborhoodIteratorType::RadiusType radiusMax;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
  {
    radiusMax[i] = lowPassOperator.GetRadius()[i];
    if ( radius[i] < highPassOperator.GetRadius()[i] )
      radius[i] = highPassOperator.GetRadius()[i];
  }

  FaceCalculatorType faceCalculator;  
  FaceListType faceList;
  faceList = faceCalculator( imgHighPass, inputRegionForThread, radiusMax );

  InnerProductType innerProduct;

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    IteratorType out ( this->GetOutput(), *faceIt );

    NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), imgLowPass, *faceIt );
    NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), imgHighPass, *faceIt );

    lowIter.GoToBegin();
    highIter.GoToBegin();
    out.GoToBegin();

    while ( !out.IsAtEnd() )
    {  
      out.Set( innerProduct( lowIter, lowPassOperator ) 
                + innerProduct( highIter, highPassOperator ) );

      ++lowIter;
      ++highIter;
      ++out;

      reporter.CompletedPixel();
    }
  }
}




} // end of namespace

#endif




