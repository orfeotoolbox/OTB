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

#ifndef __otbWaveletFilterBank_txx
#define __otbWaveletFilterBank_txx
#include "otbWaveletFilterBank.h"

#include "otbMacro.h"
#include "otbSubsampleImageFilter.h"
#include "otbSubsampledImageRegionConstIterator.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkPeriodicBoundaryCondition.h"
#include "otbMirrorBoundaryCondition.h"
#include "itkProgressReporter.h"

// FIXME
#define __myDebug__ 0 
#include "otbImageFileWriter.h"

namespace otb {

/**
 * Template Specialisation for the FORWARD case
 */

template < class TInputImage, class TOutputImage, class TWaveletOperator >
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::WaveletFilterBank ()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfInputs(1);

  unsigned int numOfOutputs = 1<<InputImageDimension;

  this->SetNumberOfOutputs( numOfOutputs );
  for ( unsigned int i = 0; i < numOfOutputs; i++ )
  {
    this->SetNthOutput(i,OutputImageType::New());
  }
  
  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::GenerateOutputInformation ()
{
  Superclass::GenerateOutputInformation();

  if ( GetSubsampleImageFactor() == 1 )
    return;


#if __myDebug__ 
  otbGenericMsgDebugMacro( << " down sampling output regions by a factor of " << GetSubsampleImageFactor() );
  otbGenericMsgDebugMacro( << "initial region    " << this->GetInput()->GetLargestPossibleRegion().GetSize()[0]
    << "," << this->GetInput()->GetLargestPossibleRegion().GetSize()[1] );
#endif

  OutputImageRegionType newRegion;
  this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );

  for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); i++ )
  {
    this->GetOutput(i)->SetRegions( newRegion );
  }

#if __myDebug__
  otbGenericMsgDebugMacro( << "new region output " << newRegion.GetSize()[0] << "," << newRegion.GetSize()[1] );
#endif
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::GenerateInputRequestedRegion () 
  throw ( itk::InvalidRequestedRegionError )
{
  Superclass::GenerateInputRequestedRegion();

  // Filter length calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(0);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  unsigned int radius = lowPassOperator.GetRadius()[0];

  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(0);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  if ( radius < highPassOperator.GetRadius()[0] )
    radius = highPassOperator.GetRadius()[0];

  // Get the requested regionand pad it
  InputImagePointerType input = const_cast< InputImageType* >( this->GetInput() );
  InputImageRegionType inputRegion = input->GetRequestedRegion();
  inputRegion.PadByRadius( radius );

  if ( inputRegion.Crop( input->GetLargestPossibleRegion() ) )
  {
    input->SetRequestedRegion( inputRegion );
  }
  else
  {
    input->SetRequestedRegion( inputRegion );
    itk::InvalidRequestedRegionError err ( __FILE__, __LINE__ );
    err.SetLocation(ITK_LOCATION);
    err.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    err.SetDataObject( input );
    throw err;
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::BeforeThreadedGenerateData ()
{
  if ( m_SubsampleImageFactor > 1 && InputImageDimension > 1 )
  {
    // Internal images will be used only if m_SubsampledInputImages != 1
    m_InternalImages.resize( InputImageDimension-1 );
    for ( unsigned int i = 0; i < m_InternalImages.size(); i++ )
    {
      // the size is linked to the SubsampleImageFactor that is assume to be 2!!!
      m_InternalImages[InputImageDimension-2-i].resize( 1<<(i+1) );
    }

    OutputImageRegionType intermediateRegion;
    this->Superclass::CallCopyInputRegionToOutputRegion( intermediateRegion, 
      this->GetInput()->GetLargestPossibleRegion() );

    AllocateInternalData( intermediateRegion );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::AllocateInternalData 
( const OutputImageRegionType& outputRegion )
{
  OutputImageRegionType smallerRegion, largerRegion = outputRegion;

  for ( unsigned int direction = 0; direction < InputImageDimension-1; direction++ )
  {
    this->CallCopyInputRegionToOutputRegion( InputImageDimension-1-direction, 
        smallerRegion, largerRegion );

    for ( unsigned int i = 0; 
        i < m_InternalImages[direction].size(); 
        ++i )
    {
      m_InternalImages[InputImageDimension-2-direction][i] = OutputImageType::New();
      m_InternalImages[InputImageDimension-2-direction][i]->SetRegions( smallerRegion );
      m_InternalImages[InputImageDimension-2-direction][i]->Allocate();
      m_InternalImages[InputImageDimension-2-direction][i]->FillBuffer(0);
    }

    largerRegion = smallerRegion;
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::AfterThreadedGenerateData ()
{
  if ( m_SubsampleImageFactor > 1 && InputImageDimension > 1 )
  {
    m_InternalImages.clear();
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
      destSize[i] = srcSize[i] * GetSubsampleImageFactor();
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::CallCopyOutputRegionToInputRegion 
( unsigned int direction,
  InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      if ( i == direction )
      {
        destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
        destSize[i] = srcSize[i] * GetSubsampleImageFactor();
      }
      else
      {
        destIndex[i] = srcIndex[i] ;
        destSize[i] = srcSize[i] ;
      }
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
      destSize[i] = srcSize[i] / GetSubsampleImageFactor();
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::CallCopyInputRegionToOutputRegion
( unsigned int direction,
  OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      if ( i == direction )
      {
        destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
        destSize[i] = srcSize[i] / GetSubsampleImageFactor();
      }
      else
      {
        destIndex[i] = srcIndex[i] ;
        destSize[i] = srcSize[i] ;
      }
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::ThreadedGenerateData 
( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  unsigned int dir = InputImageDimension-1;

  if ( (1<<dir) >= static_cast<int>( this->GetNumberOfOutputs() ) )
  {
    itk::OStringStream msg;
    msg << "Output number 1<<" << dir << " = " << (1<<dir) << " not allocated\n";
    msg << "Number of excpected outputs " << this->GetNumberOfOutputs() << "\n";
    throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );
  }

  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfOutputs() * 2 );

  const InputImageType * input = this->GetInput();
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  OutputImagePointerType outputLowPass = this->GetOutput( 0 );
  OutputImagePointerType outputHighPass = this->GetOutput( 1<<dir );

  // On multidimensional case, if m_SubsampleImageFactor != 1, we need internal images of different size
  if ( dir != 0 && m_SubsampleImageFactor > 1 )
  {
    outputLowPass = m_InternalImages[dir-1][0];
    outputHighPass = m_InternalImages[dir-1][1];
  }

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< InputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< OutputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  // Prepare the subsampling image factor, if any.
  typedef SubsampledImageRegionConstIterator< InputImageType > SubsampleIteratorType;
  typename SubsampleIteratorType::IndexType subsampling;
  subsampling.Fill(1);
  subsampling[dir] = GetSubsampleImageFactor();

  // Inner product
  InnerProductType innerProduct;
  FaceCalculatorType faceCalculator;  
  FaceListType faceList;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(dir);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  faceList = faceCalculator( input, inputRegionForThread, highPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    SubsampleIteratorType subIt ( input, *faceIt );
    subIt.SetSubsampleFactor( subsampling );
    subIt.GoToBegin();

    NeighborhoodIteratorType it ( highPassOperator.GetRadius(), input, *faceIt );
    //itk::PeriodicBoundaryCondition< InputImageType > boundaryCondition;
    //otb::MirrorBoundaryCondition< InputImageType > boundaryCondition;
    //it.OverrideBoundaryCondition( &boundaryCondition );

    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion( dir, outputImageRegion, *faceIt );
    IteratorType out ( outputHighPass, outputImageRegion );
    out.GoToBegin();

    while ( !subIt.IsAtEnd() && !out.IsAtEnd() )
    {
      it.SetLocation( subIt.GetLocationIndex() );
      out.Set( innerProduct( it, highPassOperator ) );

      ++subIt;
      ++out;

      reporter.CompletedPixel();
    }
  }

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(dir);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  faceList = faceCalculator( input, inputRegionForThread, lowPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    SubsampleIteratorType subIt ( input, *faceIt );
    subIt.SetSubsampleFactor( subsampling );
    subIt.GoToBegin();

    NeighborhoodIteratorType it ( lowPassOperator.GetRadius(), input, *faceIt );
    //itk::PeriodicBoundaryCondition< InputImageType > boundaryCondition;
    //otb::MirrorBoundaryCondition< InputImageType > boundaryCondition;
    //it.OverrideBoundaryCondition( &boundaryCondition );

    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion( dir, outputImageRegion, *faceIt );
    IteratorType out ( outputLowPass, outputImageRegion );
    out.GoToBegin();

    while ( !subIt.IsAtEnd() && !out.IsAtEnd() )
    {
      it.SetLocation( subIt.GetLocationIndex() );
      out.Set( innerProduct( it, lowPassOperator ) );

      ++subIt;
      ++out;

      reporter.CompletedPixel();
    }
  }

  if ( dir > 0 )
  { 
    // Note that outputImageRegion correspond to the actual region of (local) input !
    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion( dir, outputImageRegion, inputRegionForThread );

    ThreadedGenerateDataAtDimensionN( 0, dir-1, reporter, outputImageRegion, threadId );
    ThreadedGenerateDataAtDimensionN( 1<<dir, dir-1, reporter, outputImageRegion, threadId );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, FORWARD >
::ThreadedGenerateDataAtDimensionN
( unsigned int idx, unsigned int direction, itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  // Note that outputRegionForThread correspond to the actual region of input !
  OutputImagePointerType input = this->GetOutput( idx );
  OutputImagePointerType outputHighPass = this->GetOutput( idx + (1<<direction) );
  OutputImagePointerType outputLowPass = OutputImageType::New();

  OutputImageRegionType outputImageRegion;
  this->CallCopyInputRegionToOutputRegion( direction, outputImageRegion, outputRegionForThread );

  if ( m_SubsampleImageFactor > 1 )
  {
    input = m_InternalImages[direction][ idx / (1<<(direction+1))];

    if ( direction != 0 )
    {
      outputLowPass = m_InternalImages[direction-1][ idx / (1<<direction) ];
      outputHighPass = m_InternalImages[direction-1][ idx / (1<<direction) + 1];
    }
  }

  if ( direction == 0 )
  {
    // The output image has to be allocated
    // May be not valid on multithreaded process ?
    outputLowPass->SetRegions( outputImageRegion );
    outputLowPass->Allocate();
    outputLowPass->FillBuffer(0);
  }

  /** Inner product iterators */
  typedef itk::ConstNeighborhoodIterator< OutputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< OutputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< OutputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm  
    ::ImageBoundaryFacesCalculator< OutputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  // Prepare the subsampling image factor, if any.
  typedef SubsampledImageRegionConstIterator< InputImageType > SubsampleIteratorType;
  typename SubsampleIteratorType::IndexType subsampling;
  subsampling.Fill(1);
  subsampling[ direction ] = GetSubsampleImageFactor();

  // Inner products
  InnerProductType innerProduct;
  FaceCalculatorType faceCalculator;  
  FaceListType faceList;

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(direction);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  // Faces iterations
  faceList = faceCalculator( input, outputRegionForThread, highPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    SubsampleIteratorType subIt ( input, *faceIt );
    subIt.SetSubsampleFactor( subsampling );
    subIt.GoToBegin();

    NeighborhoodIteratorType it ( highPassOperator.GetRadius(), input, *faceIt );
    //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
    //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
    //it.OverrideBoundaryCondition( &boundaryCondition );
    
    OutputImageRegionType localOutputRegion;
    this->CallCopyInputRegionToOutputRegion( direction, localOutputRegion, *faceIt );
    IteratorType out ( outputHighPass, localOutputRegion );
    out.GoToBegin();

    while ( !subIt.IsAtEnd() && !out.IsAtEnd() )
    {
      it.SetLocation( subIt.GetLocationIndex() );
      out.Set( innerProduct( it, highPassOperator ) );

      ++subIt;
      ++out;

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
    SubsampleIteratorType subIt ( input, *faceIt );
    subIt.SetSubsampleFactor( subsampling );
    subIt.GoToBegin();

    NeighborhoodIteratorType it ( lowPassOperator.GetRadius(), input, *faceIt );
    //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
    //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
    //it.OverrideBoundaryCondition( &boundaryCondition );

    OutputImageRegionType localOutputRegion;
    this->CallCopyInputRegionToOutputRegion( direction, localOutputRegion, *faceIt );
    IteratorType out ( outputLowPass, localOutputRegion );
    out.GoToBegin();

    while ( !subIt.IsAtEnd() && !out.IsAtEnd() )
    {
      it.SetLocation( subIt.GetLocationIndex() );
      out.Set( innerProduct( it, lowPassOperator ) );

      ++subIt;
      ++out;

      reporter.CompletedPixel();
    }
  }

  // Swap input and lowPassOutput
  itk::ImageRegionConstIterator< OutputImageType > inIt ( outputLowPass, outputImageRegion );
  IteratorType outIt ( ( direction != 0 && m_SubsampleImageFactor > 1 ) ? 
                         static_cast<OutputImageType*>( m_InternalImages[direction-2][idx/(1<<(direction-1))] ) 
                          : this->GetOutput( idx ), 
                        outputImageRegion );

  for ( inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt, ++outIt )
  {
    outIt.Set( inIt.Get() );
  }

  if ( direction != 0 )
  {
    ThreadedGenerateDataAtDimensionN( idx, direction-1, reporter, outputImageRegion, threadId );
    ThreadedGenerateDataAtDimensionN( idx + (1<<direction), direction-1, reporter, outputImageRegion, threadId );
  }
}

/**
 * Template Specialisation for the INVERSE case
 */

template < class TInputImage, class TOutputImage, class TWaveletOperator >
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::WaveletFilterBank ()
{
  this->SetNumberOfRequiredInputs( 1 << InputImageDimension );
  this->SetNumberOfInputs( 1 << InputImageDimension );
  this->SetNumberOfOutputs(1);

  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;
}


template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  for ( unsigned int i = 1; i < this->GetNumberOfInputs(); i++ )
  {
    for ( unsigned int dim = 0; dim < InputImageDimension; dim++ )
    {
      if ( this->GetInput(0)->GetLargestPossibleRegion().GetSize()[dim]
          != this->GetInput(i)->GetLargestPossibleRegion().GetSize()[dim] )
      {
        throw itk::ExceptionObject( __FILE__, __LINE__, 
          "Input images are not of the same dimension", ITK_LOCATION );
      }
    }
  }

  //if ( GetSubsampleImageFactor() == 1 )
    //return;

#if __myDebug__
  otbGenericMsgDebugMacro( << " up sampling output regions by a factor of " << GetSubsampleImageFactor() );

  otbGenericMsgDebugMacro( << "initial region    " << this->GetInput(0)->GetLargestPossibleRegion().GetSize()[0]
    << "," << this->GetInput(0)->GetLargestPossibleRegion().GetSize()[1] );
#endif

  OutputImageRegionType newRegion;
  this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput(0)->GetLargestPossibleRegion() );
  this->GetOutput()->SetRegions( newRegion );

#if __myDebug__
  otbGenericMsgDebugMacro( << "new region output " << newRegion.GetSize()[0] << "," << newRegion.GetSize()[1] );
#endif
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::GenerateInputRequestedRegion () 
  throw ( itk::InvalidRequestedRegionError )
{
  Superclass::GenerateInputRequestedRegion();

  // Filter length calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(0);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  unsigned int radius = lowPassOperator.GetRadius()[0];

  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(0);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  if ( radius < highPassOperator.GetRadius()[0] )
    radius = highPassOperator.GetRadius()[0];

  // Get the requested regionand pad it
  for ( unsigned int idx = 0; idx < this->GetNumberOfInputs(); idx++ )
  {
    InputImagePointerType input = const_cast< InputImageType* >( this->GetInput(idx) );
    InputImageRegionType inputRegion = input->GetRequestedRegion();
    inputRegion.PadByRadius( radius );

    if ( inputRegion.Crop( input->GetLargestPossibleRegion() ) )
    {
      input->SetRequestedRegion( inputRegion );
    }
    else
    {
      input->SetRequestedRegion( inputRegion );
      itk::InvalidRequestedRegionError err ( __FILE__, __LINE__ );
      err.SetLocation(ITK_LOCATION);
      err.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      err.SetDataObject( input );
      throw err;
    }
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::BeforeThreadedGenerateData ()
{
  if ( InputImageDimension > 1 )
  {
    // Internal images will be used only if m_SubsampleImageFactor != 1
    m_InternalImages.resize( InputImageDimension-1 );
    for ( unsigned int i = 0; i < m_InternalImages.size(); i++ )
    {
      // the size is linked to the SubsampleImageFactor that is assume to be 2!!!
      m_InternalImages[i].resize( 1<<(i+1) );
    }

    OutputImageRegionType intermediateRegion;
    Superclass::CallCopyInputRegionToOutputRegion( intermediateRegion, 
      this->GetInput(0)->GetLargestPossibleRegion() );

    AllocateInternalData( intermediateRegion );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::AllocateInternalData 
( const OutputImageRegionType& outputRegion )
{
  OutputImageRegionType largerRegion, smallerRegion = outputRegion;

  for ( unsigned int direction = 0; direction < InputImageDimension-1; direction++ )
  {
    this->CallCopyInputRegionToOutputRegion( direction, 
        largerRegion, smallerRegion );

    for ( unsigned int i = 0; 
        i < m_InternalImages[direction].size(); 
        i++ )
    {
      m_InternalImages[direction][i] = OutputImageType::New();
      m_InternalImages[direction][i]->SetRegions( largerRegion );
      m_InternalImages[direction][i]->Allocate();
      m_InternalImages[direction][i]->FillBuffer(0);
    }

    smallerRegion = largerRegion;
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::AfterThreadedGenerateData ()
{
  if ( m_SubsampleImageFactor > 1 && InputImageDimension > 1 )
  {
    m_InternalImages.clear();
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
      destSize[i] = srcSize[i] / GetSubsampleImageFactor();
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );

  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
      destSize[i] = srcSize[i] * GetSubsampleImageFactor();
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::CallCopyOutputRegionToInputRegion 
( unsigned int direction,
  InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      if ( i == direction )
      {
        destIndex[i] = srcIndex[i] / GetSubsampleImageFactor();
        destSize[i] = srcSize[i] / GetSubsampleImageFactor();
      }
      else
      {
        destIndex[i] = srcIndex[i] ;
        destSize[i] = srcSize[i] ;
      }
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::CallCopyInputRegionToOutputRegion
( unsigned int direction,
  OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      if ( i == direction )
      {
        destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
        destSize[i] = srcSize[i] * GetSubsampleImageFactor();
      }
      else
      {
        destIndex[i] = srcIndex[i] ;
        destSize[i] = srcSize[i] ;
      }
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::ThreadedGenerateData 
( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfInputs() );

  InputImageRegionType inputRegionForThread;
	this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );
  
  unsigned int dir = 0; 

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(dir);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(dir);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator< OutputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< OutputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< OutputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< OutputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  // Faces iterations
  typename NeighborhoodIteratorType::RadiusType radiusMax ;
  for ( unsigned int idx = 0; idx < OutputImageDimension; idx++ )
  {
    radiusMax[idx] = lowPassOperator.GetRadius(idx);
    if ( radiusMax[idx] < highPassOperator.GetRadius(idx) )
      radiusMax[idx] = highPassOperator.GetRadius(idx);
  }


  // The multiresolution case requires a SubsampleImageFilter step
  if ( m_SubsampleImageFactor > 1 ) 
  {
    for ( unsigned int i = 0; i < this->GetNumberOfInputs(); i+=2 )
    {
      InputImagePointerType imgLowPass = const_cast<InputImageType*>( this->GetInput(i) );
      InputImagePointerType imgHighPass = const_cast<InputImageType*>( this->GetInput(i+1) );

      OutputImagePointerType outputImage = this->GetOutput();
      if ( dir != InputImageDimension-1 )
      {
        outputImage = m_InternalImages[0][i/2];
      }

      typedef otb::SubsampleImageFilter< InputImageType, OutputImageType, INVERSE > FilterType;
      typename FilterType::InputImageIndexType delta;
      delta.Fill(1);
      delta[ dir ] = this->GetSubsampleImageFactor();

      typename FilterType::Pointer overSampledLowPass = FilterType::New();
      overSampledLowPass->SetInput( imgLowPass );
      overSampledLowPass->SetSubsampleFactor( delta );
      overSampledLowPass->Update();

      typename FilterType::Pointer overSampledHighPass = FilterType::New();
      overSampledHighPass->SetInput( imgHighPass );
      overSampledHighPass->SetSubsampleFactor( delta );
      overSampledHighPass->Update();

      FaceCalculatorType faceCalculator;  
      FaceListType faceList;
      faceList = faceCalculator( overSampledLowPass->GetOutput(), 
                                  overSampledLowPass->GetOutput()->GetRequestedRegion(), 
                                  radiusMax );

      InnerProductType innerProduct;

      for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
      {
        itk::ImageRegionIterator< OutputImageType > out ( outputImage, *faceIt );

        //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
        //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
        NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), overSampledLowPass->GetOutput(), *faceIt );
        //lowIter.OverrideBoundaryCondition( &boundaryCondition );

        NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), overSampledHighPass->GetOutput(), *faceIt );
        //highIter.OverrideBoundaryCondition( &boundaryCondition );

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
    } // end for each overSampledLowPass/overSampledhighPass pair of entry
  }
  else // multiscale case
  {
    for ( unsigned int i = 0; i < this->GetNumberOfInputs(); i+=2 )
    {
      InputImagePointerType imgLowPass = const_cast<InputImageType*>( this->GetInput(i) );
      InputImagePointerType imgHighPass = const_cast<InputImageType*>( this->GetInput(i+1) );

      OutputImagePointerType outputImage = this->GetOutput();
      if ( dir != InputImageDimension-1 )
      {
        outputImage = m_InternalImages[0][i/2];
      }

      FaceCalculatorType faceCalculator;  
      FaceListType faceList;
      faceList = faceCalculator( imgLowPass, imgLowPass->GetRequestedRegion(), radiusMax );

      InnerProductType innerProduct;

      for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
      {
        itk::ImageRegionIterator< OutputImageType > out ( outputImage, *faceIt );

        //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
        //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
        NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), imgLowPass, *faceIt );
        //lowIter.OverrideBoundaryCondition( &boundaryCondition );

        NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), imgHighPass, *faceIt );
        //highIter.OverrideBoundaryCondition( &boundaryCondition );

        lowIter.GoToBegin();
        highIter.GoToBegin();
        out.GoToBegin();

        while ( !out.IsAtEnd() )
        {  
          out.Set( ( innerProduct( lowIter, lowPassOperator ) 
                     + innerProduct( highIter, highPassOperator ) ) / 2. );

          ++lowIter;
          ++highIter;
          ++out;

          reporter.CompletedPixel();
        }
      }
    } // end for each imgLowPass/imghighPass pair of entry
  } // end multiscale case

  if ( dir != InputImageDimension-1 )
  {
    // Note that outputImageRegion correspond to the actual region of (local) input !
    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion( dir, outputImageRegion, inputRegionForThread );

    ThreadedGenerateDataAtDimensionN( dir+1, reporter, outputImageRegion, threadId );
  }

}

template < class TInputImage, class TOutputImage, class TWaveletOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, TWaveletOperator, INVERSE >
::ThreadedGenerateDataAtDimensionN
( unsigned int direction, 
  itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageRegionType outputImageRegion;
  this->CallCopyInputRegionToOutputRegion( direction, outputImageRegion, outputRegionForThread );

  // Low pass part calculation
  LowPassOperatorType lowPassOperator;
  lowPassOperator.SetDirection(direction);
  lowPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  lowPassOperator.CreateDirectional();

  // High pass part calculation
  HighPassOperatorType highPassOperator;
  highPassOperator.SetDirection(direction);
  highPassOperator.SetUpSampleFactor( this->GetUpSampleFilterFactor() );
  highPassOperator.CreateDirectional();

  // typedef for the iterations over the input image
  typedef itk::ConstNeighborhoodIterator< OutputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< OutputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< OutputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< OutputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  // Faces iterations
  typename NeighborhoodIteratorType::RadiusType radiusMax;
  for ( unsigned int i = 0; i < InputImageDimension; i++ )
  {
    radiusMax[i] = lowPassOperator.GetRadius(i);
    if ( radiusMax[i] < highPassOperator.GetRadius(i) )
      radiusMax[i] = highPassOperator.GetRadius(i);
  }

  // The multiresolution case requires a SubsampleImageFilter step
  if ( m_SubsampleImageFactor > 1 ) 
  {
    for ( unsigned int i = 0; i < m_InternalImages[direction-1].size(); i+=2 )
    {
      InputImagePointerType imgLowPass = m_InternalImages[direction-1][i];
      InputImagePointerType imgHighPass = m_InternalImages[direction-1][i+1];

      OutputImagePointerType outputImage = this->GetOutput();
      if ( direction < InputImageDimension-1 )
      {
        outputImage = m_InternalImages[direction][i/2];
      }

      typedef otb::SubsampleImageFilter< OutputImageType, OutputImageType, INVERSE > FilterType;
      typename FilterType::InputImageIndexType delta;
      delta.Fill(1);
      delta[ direction ] = this->GetSubsampleImageFactor();

      typename FilterType::Pointer overSampledLowPass = FilterType::New();
      overSampledLowPass->SetInput( imgLowPass );
      overSampledLowPass->SetSubsampleFactor( delta );
      overSampledLowPass->Update();

      typename FilterType::Pointer overSampledHighPass = FilterType::New();
      overSampledHighPass->SetInput( imgHighPass );
      overSampledHighPass->SetSubsampleFactor( delta );
      overSampledHighPass->Update();

      InnerProductType innerProduct;
      FaceCalculatorType faceCalculator;  
      FaceListType faceList;
      faceList = faceCalculator( overSampledLowPass->GetOutput(), 
                                  overSampledLowPass->GetOutput()->GetRequestedRegion(), 
                                  radiusMax );

      for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
      {
        itk::ImageRegionIterator< OutputImageType > out ( outputImage, *faceIt );

        //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
        //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
        NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), overSampledLowPass->GetOutput(), *faceIt );
        //lowIter.OverrideBoundaryCondition( &boundaryCondition );

        NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), overSampledHighPass->GetOutput(), *faceIt );
        //highIter.OverrideBoundaryCondition( &boundaryCondition );

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
    } // end for each overSampledLowPass/overSampledhighPass pair of entry
  }
  else // multiscale case
  {
    for ( unsigned int i = 0; i < m_InternalImages[direction-1].size(); i+=2 )
    {
      InputImagePointerType imgLowPass = m_InternalImages[direction-1][i];
      InputImagePointerType imgHighPass = m_InternalImages[direction-1][i+1];

      OutputImagePointerType outputImage = this->GetOutput();
      if ( direction < InputImageDimension-1 )
      {
        outputImage = m_InternalImages[direction][i/2];
      }

      InnerProductType innerProduct;
      FaceCalculatorType faceCalculator;  
      FaceListType faceList;
      faceList = faceCalculator( imgLowPass, imgLowPass->GetRequestedRegion(), radiusMax );

      for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
      {
        itk::ImageRegionIterator< OutputImageType > out ( outputImage, *faceIt );

        //itk::PeriodicBoundaryCondition< OutputImageType > boundaryCondition;
        //otb::MirrorBoundaryCondition< OutputImageType > boundaryCondition;
        NeighborhoodIteratorType lowIter ( lowPassOperator.GetRadius(), imgLowPass, *faceIt );
        //lowIter.OverrideBoundaryCondition( &boundaryCondition );

        NeighborhoodIteratorType highIter ( highPassOperator.GetRadius(), imgHighPass, *faceIt );
        //highIter.OverrideBoundaryCondition( &boundaryCondition );

        lowIter.GoToBegin();
        highIter.GoToBegin();
        out.GoToBegin();

        while ( !out.IsAtEnd() )
        {  
          out.Set( ( innerProduct( lowIter, lowPassOperator ) 
                      + innerProduct( highIter, highPassOperator ) ) / 2. );

          ++lowIter;
          ++highIter;
          ++out;

          reporter.CompletedPixel();
        }
      }
    } // end for each imgLowPass/imghighPass pair of entry
  }

  if ( direction < InputImageDimension-1 )
  {
    ThreadedGenerateDataAtDimensionN( direction+1, reporter, outputImageRegion, threadId );
  }
}

} // end of namespace

#endif




