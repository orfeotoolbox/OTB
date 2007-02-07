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
#ifndef _otbStreamingShrinkImageFilter_txx
#define _otbStreamingShrinkImageFilter_txx

#include "otbStreamingShrinkImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/** Constructor */
template <class TImage>
StreamingShrinkImageFilter<TImage>
::StreamingShrinkImageFilter()
{
  // Default shrink factor
  m_ShrinkFactor=10;
}
/** Destructor */
template <class TImage>
StreamingShrinkImageFilter<TImage>
::~StreamingShrinkImageFilter()
{}
/** 
 * StreamingShrinkImageFilter produces an ouptut whose size is different from its input.
 * As such, it must override the GenerateOutputInformation method in order to compute
 * the output size from the input size.
 */
template <class TImage>
void
StreamingShrinkImageFilter<TImage>
::GenerateOutputInformation(void)
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const typename ImageType::SpacingType&
    inputSpacing = inputPtr->GetSpacing();
  const typename ImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename ImageType::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();
  otbMsgDebugMacro(<<"Input idnex "<<inputStartIndex);
  otbMsgDebugMacro(<<"Input size: "<<inputSize);

  typename ImageType::SpacingType      outputSpacing;
  typename ImageType::SizeType         outputSize;
  typename ImageType::IndexType        outputStartIndex;

  for (unsigned int i = 0; i < ImageType::ImageDimension; i++)
    {
      outputSpacing[i] = inputSpacing[i] * static_cast<double>( m_ShrinkFactor);
      outputSize[i] = inputSize[i]/m_ShrinkFactor;
      outputStartIndex[i] = inputStartIndex[i];
    }
  outputPtr->SetSpacing( outputSpacing );
  typename ImageType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );
  otbMsgDebugMacro(<<"Output largest possible region: "<<outputLargestPossibleRegion);
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

/** Main computation method */
template <class TImage>
void
StreamingShrinkImageFilter<TImage>
::UpdateOutputData(itk::DataObject *itkNotUsed(output))
{
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
  ImagePointerType outputPtr = this->GetOutput(0);
  typename ImageType::RegionType outputRegion = outputPtr->GetRequestedRegion();
  outputPtr->SetBufferedRegion( outputRegion );
  outputPtr->Allocate();
  
  /**
   * Grab the input
   */
  ImagePointerType inputPtr =  const_cast<ImageType * >( this->GetInput(0) );
  
  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;

  IteratorType it(outputPtr,outputRegion);
  it.GoToBegin();
  
  typename ImageType::SizeType size = outputRegion.GetSize();

  for(unsigned int i=0;i<size[1]&&!it.IsAtEnd();++i)
	{
	  typename ImageType::IndexType readIndex;
	  readIndex[0] = 0;
	  readIndex[1] = i*m_ShrinkFactor;
	  typename ImageType::SizeType readSize;
	  readSize[0]=size[0]*m_ShrinkFactor;
	  readSize[1]=1;
	  typename ImageType::RegionType readRegion;
	  readRegion.SetSize(readSize);
	  readRegion.SetIndex(readIndex);
	  inputPtr->SetRequestedRegion(readRegion);
	  inputPtr->PropagateRequestedRegion();
	  inputPtr->UpdateOutputData();
	  IteratorType readIt(inputPtr,readRegion);
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
template <class TImage>
void
StreamingShrinkImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Shrink facotr: " << m_ShrinkFactor
     << std::endl;
}
} // End namespace otb
#endif
