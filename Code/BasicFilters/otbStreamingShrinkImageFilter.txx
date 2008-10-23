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
#ifndef __otbStreamingShrinkImageFilter_txx
#define __otbStreamingShrinkImageFilter_txx

#include "otbStreamingShrinkImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TOutputImage>
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::StreamingShrinkImageFilter()
{
  // Default shrink factor
  m_ShrinkFactor=10;
}
/** Destructor */
template <class TInputImage, class TOutputImage>
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::~StreamingShrinkImageFilter()
{}
/** 
 * StreamingShrinkImageFilter produces an output whose size is different from its input.
 * As such, it must override the GenerateOutputInformation method in order to compute
 * the output size from the input size.
 */
template <class TInputImage, class TOutputImage>
void
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const typename InputImageType::SpacingType&
    inputSpacing = inputPtr->GetSpacing();
  const typename InputImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename InputImageType::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  otbMsgDebugMacro(<<"Input index "<<inputStartIndex);
  otbMsgDebugMacro(<<"Input size: "<<inputSize);

  typename OutputImageType::SpacingType      outputSpacing;
  typename OutputImageType::SizeType         outputSize;
  typename OutputImageType::IndexType        outputStartIndex;

  for (unsigned int i = 0; i < OutputImageType::ImageDimension; i++)
    {
      outputSpacing[i] = inputSpacing[i] * static_cast<double>( m_ShrinkFactor);
      outputSize[i] = static_cast<int>( static_cast<double>(inputSize[i])/static_cast<double>( m_ShrinkFactor )) ;
      //outputStartIndex[i] = inputStartIndex[i];
      outputStartIndex[i]=0;
    }
  outputPtr->SetSpacing( outputSpacing );
  typename OutputImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );
  // otbMsgDebugMacro(<<"Output largest possible region: "<<outputLargestPossibleRegion);
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template <class TInputImage, class TOutputImage>
void
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion(void)
{
  // otbMsgDebugMacro(<<"Call to GenerateInputRequestedRegion");
  // if(this->GetInput())
//     {
      InputImagePointerType inputPtr =  const_cast<InputImageType * >( this->GetInput(0) );
      // otbMsgDebugMacro(<<"Input largest possible region: "<<inputPtr->GetLargestPossibleRegion());
      typename InputImageType::IndexType index = inputPtr->GetLargestPossibleRegion().GetIndex();
      typename InputImageType::SizeType size;
      size.Fill(0);
      typename InputImageType::RegionType region;
      region.SetSize(size);
      region.SetIndex(index);
      inputPtr->SetRequestedRegion(region);
//     }
}

/** Main computation method */
template <class TInputImage, class TOutputImage>
void
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::UpdateOutputData(itk::DataObject *itkNotUsed(output))
{
  // otbMsgDebugMacro(<<"Entering UpdateOutputData");
  /**
   * prevent chasing our tail
   */
  if (this->m_Updating)
    {
    return;
    }
  /**
   * Prepare all the outputs. This may deallocate previous bulk data.
   */
  this->PrepareOutputs();

  /**
   * Make sure we have the necessary inputs
   */
  unsigned int ninputs = this->GetNumberOfValidRequiredInputs();
  if (ninputs < 1)
    {
    itkExceptionMacro(<< "At least 1  input is required but only " << ninputs << " are specified.");
    return;
    }
  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);
  this->m_Updating = true;
  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent(itk::StartEvent());
  /**
   * Allocate the output buffer. 
   */
  OutputImagePointerType outputPtr = this->GetOutput(0);
  typename OutputImageType::RegionType outputRegion = outputPtr->GetRequestedRegion();
  outputPtr->SetBufferedRegion( outputRegion );
  outputPtr->Allocate();
  
  /**
   * Grab the input
   */
  InputImagePointerType inputPtr =  const_cast<InputImageType * >( this->GetInput(0) );
  
  // otbMsgDebugMacro(<<"Input Largest possible region: "<<inputPtr->GetLargestPossibleRegion());
  typename InputImageType::IndexType origin = inputPtr->GetLargestPossibleRegion().GetIndex();

  typedef itk::ImageRegionIteratorWithIndex<OutputImageType> OutputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<InputImageType> InputIteratorType;

  OutputIteratorType it(outputPtr,outputRegion);
  it.GoToBegin();
  
  typename OutputImageType::SizeType size = outputRegion.GetSize();

  for(unsigned int i=0;i<size[1]&&!it.IsAtEnd();++i)
	{
	  typename InputImageType::IndexType readIndex;
	  readIndex[0] = origin[0];
	  readIndex[1] = i*m_ShrinkFactor+origin[1];
	  typename InputImageType::SizeType readSize;
	  readSize[0]=size[0]*m_ShrinkFactor;
	  readSize[1]=1;
	  typename InputImageType::RegionType readRegion;
	  readRegion.SetSize(readSize);
	  readRegion.SetIndex(readIndex);
	  // otbMsgDebugMacro(<<"Read region: "<<readRegion);
//  	  otbMsgDebugMacro(<<"Largest input region: "<<inputPtr->GetLargestPossibleRegion());
	  inputPtr->SetRequestedRegion(readRegion);
	  inputPtr->PropagateRequestedRegion();
	  inputPtr->UpdateOutputData();
	  InputIteratorType readIt(inputPtr,readRegion);
	  unsigned int count=0;
	  for(readIt.GoToBegin();!readIt.IsAtEnd()&&!it.IsAtEnd();++readIt,++count)
	    {
	      if(count%m_ShrinkFactor==0)
		{
		  it.Set(readIt.Get());
		  ++it;
		}
	    }
	  this->UpdateProgress(static_cast<float>(i)/ static_cast<float>(size[1]));
	}
  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if ( !this->GetAbortGenerateData() )
    {
    this->UpdateProgress(1.0);
    }

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent() );

  /**
   * Now we have to mark the data as up to data.
   */
  if (this->GetOutput(0))
      {
	this->GetOutput(0)->DataHasBeenGenerated();
      }  
  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();
  
  // Mark that we are no longer updating the data in this filter
  this->m_Updating = false;
}
/** PrintSelf method */
template <class TInputImage, class TOutputImage>
void
StreamingShrinkImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Shrink factor: " << m_ShrinkFactor
     << std::endl;
}
} // End namespace otb
#endif
