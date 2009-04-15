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
#include "otbSubsampledImageRegionConstIterator.h"

#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"

// FIXME
#include "otbImageViewer.h" 

namespace otb {

/**
 * Template Specialisation for the FORWARD case
 */

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::WaveletFilterBank ()
{
  this->SetNumberOfRequiredInputs(1);

  unsigned int numOfOutputs = 1<<InputImageType::ImageDimension;

  this->SetNumberOfOutputs( numOfOutputs );
  for ( unsigned int i = 0; i < numOfOutputs; i++ )
  {
    this->SetNthOutput(i,OutputImageType::New());
  }
  
  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if ( GetSubsampleImageFactor() == 1 )
    return;

  otbGenericMsgDebugMacro( << " down sampling output regions by a factor of " << GetSubsampleImageFactor() );

  OutputImageRegionType newRegion;
  this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );

  for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); i++ )
  {
    this->GetOutput(i)->SetRegions( newRegion );
  }

  otbGenericMsgDebugMacro( << "initial region    " << this->GetInput()->GetLargestPossibleRegion().GetSize()[0]
    << "," << this->GetInput()->GetLargestPossibleRegion().GetSize()[1] );
  otbGenericMsgDebugMacro( << "new region output " << newRegion.GetSize()[0] << "," << newRegion.GetSize()[1] );
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::BeforeThreadedGenerateData ()
{
  if ( m_SubsampleImageFactor > 1 && InputImageDimension > 1 )
  {
    // Internal images will be used only if m_SubsampledInputImages != 1
      std::cerr << "Allocating  m_InternalImages with size " << (InputImageType::ImageDimension-1) << " \n";
    m_InternalImages.resize( InputImageType::ImageDimension-1 );
    for ( unsigned int i = 0; i < m_InternalImages.size(); i++ )
    {
      // the size is liked to the SubsampleImageFactor that is assume to be 2!!!
      std::cerr << "Allocating  m_InternalImages[" << i << "] with size " << (1<<(i+1)) << " \n";
      m_InternalImages[InputImageType::ImageDimension-2-i].resize( 1<<(i+1) );
    }

    OutputImageRegionType intermediateRegion;
    Superclass::CallCopyInputRegionToOutputRegion( intermediateRegion, this->GetInput()->GetLargestPossibleRegion() );

    AllocateInternalData( intermediateRegion );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::AllocateInternalData ( const OutputImageRegionType& outputRegion )
{
  OutputImageRegionType smallerRegion, largerRegion = outputRegion;

  for ( unsigned int direction = 0; direction < InputImageType::ImageDimension-1; direction++ )
  {
    this->CallCopyInputRegionToOutputRegion( InputImageType::ImageDimension-1-direction, 
        smallerRegion, largerRegion );

    for ( unsigned int i = 0; 
        i < m_InternalImages[direction].size(); 
        ++i )
    {
      std::cerr << "SetRegion at m_InternalImages[" << (direction) << "][" << i << "]\n";

      m_InternalImages[InputImageType::ImageDimension-2-direction][i] = OutputImageType::New();
      m_InternalImages[InputImageType::ImageDimension-2-direction][i]->SetRegions( smallerRegion );
      m_InternalImages[InputImageType::ImageDimension-2-direction][i]->Allocate();
    }

    largerRegion = smallerRegion;
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::AfterThreadedGenerateData ()
{
  if ( m_SubsampleImageFactor > 1 && InputImageDimension > 1 )
  {
    m_InternalImages.clear();
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
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
      destSize[i] = ( srcSize[i] - 1 ) * GetSubsampleImageFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );

  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
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
        destSize[i] = ( srcSize[i] - 1 ) * GetSubsampleImageFactor() + 1;
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

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
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
      destSize[i] = ( srcSize[i] - 1 ) / GetSubsampleImageFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
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
        destSize[i] = ( srcSize[i] - 1 ) / GetSubsampleImageFactor() + 1;
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

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::ThreadedGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  itk::ProgressReporter reporter ( this, threadId, 
    outputRegionForThread.GetNumberOfPixels() * this->GetNumberOfOutputs() );

  const InputImageType * input = this->GetInput();
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  unsigned int dir = InputImageType::ImageDimension-1;

  OutputImageType * outputLowPass = this->GetOutput( 0 );
  OutputImageType * outputHighPass = this->GetOutput( 1<<dir );

  if ( outputLowPass == 0 )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__, "Output(0) not allocated", ITK_LOCATION );
  }

  if ( outputHighPass == 0 )
  {
    itk::OStringStream msg;
    msg << "Output number 1<<" << dir << " = " << (1<<dir) << " not allocated\n";
    msg << "Number of excpected outputs " << this->GetNumberOfOutputs() << "\n";
    throw itk::ExceptionObject( __FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION );
  }

  // On  multidimensional case, if m_SubsampleImageFactor != 1, we need internal images of different size
  if ( dir != 0 && m_SubsampleImageFactor > 1 )
  {
    std::cerr << "Using internal images [" << dir-1 << "][0] and [" << dir-1 << "][1] at thread " << threadId << "\n";
    outputLowPass = m_InternalImages[dir-1][0];
    outputHighPass = m_InternalImages[dir-1][1];
  }

  std::cerr << "InputRegion  [" << inputRegionForThread.GetSize()[0] << "," << inputRegionForThread.GetSize()[1] << "]\n";
  std::cerr << "OutputLowPass[" << outputLowPass->GetRequestedRegion().GetSize()[0] << "," << outputLowPass->GetRequestedRegion().GetSize()[1] << "]\n";
  std::cerr << "OutputHighPass[" << outputHighPass->GetRequestedRegion().GetSize()[0] << "," << outputHighPass->GetRequestedRegion().GetSize()[1] << "]\n";

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

#if 0
  // FIXME
  {
    typedef otb::ImageViewer< OutputPixelType > ViewerType;
    typename ViewerType::Pointer  lViewer = ViewerType::New();
    lViewer->SetLabel( "highPass 0" );
    lViewer->SetImage( outputHighPass );
    lViewer->Show();

    Fl::run();
  }
#endif

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

#if 0
  // FIXME
  {
    typedef otb::ImageViewer< OutputPixelType > ViewerType;
    typename ViewerType::Pointer  lViewer = ViewerType::New();
    lViewer->SetLabel( "LowPass 0" );
    lViewer->SetImage( outputLowPass );
    lViewer->Show();

    Fl::run();
  }
#endif

  if ( dir > 0 )
  { 
    // Note that outputRegionForThread correspond to the actual region of (local) input !
    OutputImageRegionType outputImageRegion;
    this->CallCopyInputRegionToOutputRegion( dir, outputImageRegion, inputRegionForThread );

    ThreadedGenerateDataAtDimensionN( 0, dir-1, reporter, outputImageRegion, threadId );
    ThreadedGenerateDataAtDimensionN( 1<<dir, dir-1, reporter, outputImageRegion, threadId );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      FORWARD >
::ThreadedGenerateDataAtDimensionN
( unsigned int idx, unsigned int direction, itk::ProgressReporter & reporter,
  const OutputImageRegionType& outputRegionForThread, int threadId )
{
  std::cerr << "-> idx = " << idx << ", direction = " << direction << "\n";

  // Note that outputRegionForThread correspond to the actual region of input !
  OutputImageType * input = this->GetOutput( idx );
  OutputImageType * outputHighPass = this->GetOutput( idx + (1<<direction) );
  OutputImagePointerType outputLowPass = OutputImageType::New();

  OutputImageRegionType outputImageRegion;
  this->CallCopyInputRegionToOutputRegion( direction, outputImageRegion, outputRegionForThread );

  if ( m_SubsampleImageFactor > 1 )
  {
    std::cerr << "Using input images [" << direction << "][" << ( idx / (1<<(direction+1)) ) << "] ";
    input = m_InternalImages[direction][ idx / (1<<(direction+1))];

    if ( direction != 0 )
    {
      std::cerr << "out LP [" << direction-1 << "][" << ( idx / (1<<direction) ) << "] ";
      outputLowPass = m_InternalImages[direction-1][ idx / (1<<direction) ];

      std::cerr << "and HP [" << direction-1 << "][" << ( idx / (1<<direction) +1 ) << "]\n";
      outputHighPass = m_InternalImages[direction-1][ idx / (1<<direction) + 1];
    }
  }

  if ( direction == 0 )
  {
    // The output image has to be allocated
    outputLowPass->SetRegions( outputImageRegion );
    outputLowPass->Allocate();
  }

#if 0
  // FIXME
  {
    typedef otb::ImageViewer< OutputPixelType > ViewerType;
    typename ViewerType::Pointer  lViewer = ViewerType::New();
    lViewer->SetLabel( "input" );
    lViewer->SetImage( input );
    lViewer->Show();

    Fl::run();
  }
#endif

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

  faceList = faceCalculator( input, outputRegionForThread, highPassOperator.GetRadius() );

  for ( FaceListIterator faceIt = faceList.begin(); faceIt != faceList.end(); ++faceIt )
  {
    SubsampleIteratorType subIt ( input, *faceIt );
    subIt.SetSubsampleFactor( subsampling );
    subIt.GoToBegin();

    NeighborhoodIteratorType it ( highPassOperator.GetRadius(), input, *faceIt );
    
    OutputImageRegionType localOutputRegion;
    this->CallCopyInputRegionToOutputRegion( direction, localOutputRegion, *faceIt );
    IteratorType out ( outputHighPass, localOutputRegion );
    out.GoToBegin();

    while ( !subIt.IsAtEnd() && !out.IsAtEnd() )
    {
      it.SetLocation( subIt.GetLocationIndex() );
        //std::cerr << "Iterateur a l'index = [" << subIt.GetLocationIndex()[0];
        //std::cerr << ", " <<  subIt.GetLocationIndex()[1] << "]\n";

      OutputPixelType tmp =  innerProduct( it, highPassOperator );
      out.Set( tmp /*innerProduct( it, highPassOperator ) */ );

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

  if ( direction != 0 && m_SubsampleImageFactor > 1 )
  {
    std::cerr << "Using tem image at [" << (direction-2) << "][" << (idx/(1<<(direction-1))) << "\n";
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


#if 0
/**
 * Template Specialisation for the INVERSE case
 */

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::WaveletFilterBank ()
{
  switch ( DirectionOfTransformation )
  {
    case FORWARD:
    {
      this->SetNumberOfRequiredInputs(1);

      unsigned int numOfOutputs = 1<<InputImageType::ImageDimension;

      this->SetNumberOfOutputs( numOfOutputs );
      m_InternalImages.resize( numOfOutputs );

      for ( unsigned i = 1; i < numOfOutputs; i++ )
      {
        this->SetNthOutput(i,OutputImageType::New());
        m_InternalImages.push_back( OutputImageType::New() )
      }
      
      break;
    }
    case INVERSE:
    {
      this->SetNumberOfRequiredOutputs(1);
      this->SetNumberOfInputs( 1 << InputImageType::ImageDimension );

      m_InternalImages.resize( this->GetNumberOfInputs() );

      for ( unsigned int i = 0; i < this->GetNumberOfInputs(); i++ )
      {
        m_InternalImages.push_back( OutputImageType::New() );
      }

      break;
    }
    default:
    {
      itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
      break;
    }
  }
  m_UpSampleFilterFactor = 0;
  m_SubsampleImageFactor = 1;
}


template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if ( GetSubsampleImageFactor() == 1 )
    return;

  switch ( DirectionOfTransformation )
  {
    case FORWARD:
    {
      otbGenericMsgDebugMacro( << " up sampling output regions by a factor of " << GetSubsampleImageFactor() );

      OutputImageRegionType newRegion;
      this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );

      for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); i++ )
      {
        this->GetOutput(i)->SetRegions( newRegion );
      }

      break;
    }
    case INVERSE:
    {
      otbGenericMsgDebugMacro( << " down sampling output regions by a factor of " << GetSubsampleImageFactor() );

      OutputImageRegionType newRegion;
      this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput(0)->GetLargestPossibleRegion() );
      this->GetOutput()->SetRegions( newRegion );
      
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
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
    switch ( DirectionOfTransformation )
    {
      case FORWARD:
      {
        EnlargeRegion( destRegion, srcRegion );
        break;
      }
      case INVERSE:
      {
        ReduceRegion( destRegion, srcRegion );
        break;
      }
      default:
      {
        itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
        break;
      }
    }
  }
}

template < class TInputImage, class TOutputImage,
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  if ( GetSubsampleImageFactor() > 1 )
  {
     switch ( DirectionOfTransformation )
    {
      case FORWARD:
      {
        ReduceRegion( destRegion, srcRegion );
        break;
      }
      case INVERSE:
      {
        EnlargeRegion( destRegion, srcRegion );
        break;
      }
      default:
      {
        itkExceptionMacro(<<"FilterBank transformation may be FORWARD or INVERSE only!!");
        break;
      }
    }
  }
}

template < class TInputImage, class TOutputImage,
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::EnlargeRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )
{
    OutputIndexType srcIndex = srcRegion.GetIndex();
    OutputSizeType srcSize = srcRegion.GetSize();

    InputIndexType destIndex;
    InputSizeType destSize;

    for ( unsigned int i = 0; i < InputImageDimension; i++ )
    {
      destIndex[i] = srcIndex[i] * GetSubsampleImageFactor();
      destSize[i] = ( srcSize[i] - 1 ) * GetSubsampleImageFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
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
      destSize[i] = ( srcSize[i] - 1 ) / GetSubsampleImageFactor() + 1;
    }

    destRegion.SetIndex( destIndex );
    destRegion.SetSize( destSize );
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::BeforeThreadedGenerateData ()
{
  if ( static_cast<int>( DirectionOfTransformation ) == INVERSE )
  {
    if ( GetSubsampleImageFactor() > 1 )
    {
      otbGenericMsgDebugMacro(<<"Subsample the " << this->GetNumberOfInputs() 
        << " inputs by a factor of " << GetSubsampleImageFactor() );
      
      for ( unsigned int i = 0; i < this->GetNumberOfInputs(); i++ )
      {
        SubsampleImageRegionConstIterator< InputImageType > subsamplingIterator 
          ( this->GetInput(i), this->GetInput(i)->GetLargestPossibleRegion() );
        subsamplingIterator.SetSubsampleFactor( GetSubsampleImageFactor() );
        subsamplingIterator.GoToBegin();

        m_SubsampledInputImages[i]->SetRegions( subsamplingIterator.GetNewRegion() );
        m_SubsampledInputImages[i]->Allocate();

        itk::ImageRegionIterator< InputImageType > iter 
          ( m_SubsampledInputImages[i], m_SubsampledInputImages[i]->GetLargestPossibleRegion() );
        iter.GoToBegin();

        while ( !subsamplingIterator.IsAtEnd() && !iter.IsAtEnd() )
        {
          iter.Set( subsamplingIterator.Get() );

          ++iter;
          ++subsamplingIterator;
        }
      }
    }
  }
}

template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
                      TLowPassOperator, THighPassOperator, 
                      TDirectionOfTransformation >
::AfterThreadedGenerateData ()
{
  if ( static_cast<int>( DirectionOfTransformation ) == FORWARD )
  {
    otbGenericMsgDebugMacro(<<"Downsample outputs by a factor of " << GetSubsampleImageFactor() );

    // Down sample inputs
  }
}


template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator,
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
void
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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
WaveletFilterBank< TInputImage, TOutputImage, 
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

#endif // Dbg


} // end of namespace

#endif




