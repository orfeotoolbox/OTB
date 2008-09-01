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
#ifndef __otbStreamingImageVirtualWriter_txx
#define __otbStreamingImageVirtualWriter_txx
#include "otbStreamingImageVirtualWriter.h"

#include "itkCommand.h"
#include "itkImageRegionIterator.h"
#include "itkObjectFactoryBase.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionMultidimensionalSplitter.h"

#include "otbMacro.h"
#include "otbConfigure.h"

namespace otb
{
/**
 *
 */
template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>
::StreamingImageVirtualWriter()
{
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  // default to 10 divisions
  m_NumberOfStreamDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  
  // create default region splitter
  m_RegionSplitter = itk::ImageRegionSplitter<InputImageDimension>::New();
}

/**
 *
 */
template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>
::~StreamingImageVirtualWriter()
{
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetBufferMemorySize(unsigned long memory_size_divisions)
{
	m_BufferMemorySize = memory_size_divisions;
	m_CalculationDivision = SET_BUFFER_MEMORY_SIZE;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetBufferNumberOfLinesDivisions(unsigned long nb_lines_divisions)
{
	m_BufferNumberOfLinesDivisions = nb_lines_divisions;
	m_CalculationDivision = SET_BUFFER_NUMBER_OF_LINES;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetNumberOfStreamDivisions(unsigned long nb_divisions)
{
	m_NumberOfStreamDivisions = nb_divisions;
	m_CalculationDivision = SET_NUMBER_OF_STREAM_DIVISIONS;
  this->Modified();
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetAutomaticNumberOfStreamDivisions(void)
{
	m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
        this->Modified();
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetTilingStreamDivisions(void)
{
	m_CalculationDivision = SET_TILING_WITH_SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
	m_RegionSplitter = itk::ImageRegionMultidimensionalSplitter<InputImageDimension>::New();
  this->Modified();
}

template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetTilingStreamDivisions(unsigned long nb_divisions)
{
	m_CalculationDivision = SET_TILING_WITH_SET_NUMBER_OF_STREAM_DIVISIONS;
	m_NumberOfStreamDivisions = nb_divisions;
	m_RegionSplitter = itk::ImageRegionMultidimensionalSplitter<InputImageDimension>::New();
  this->Modified();
}

template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::SetInput(const InputImageType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast<TInputImage *>(input ) );
}


//---------------------------------------------------------
template <class TInputImage>
const typename StreamingImageVirtualWriter<TInputImage>::InputImageType *
StreamingImageVirtualWriter<TInputImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<TInputImage*>
    (this->itk::ProcessObject::GetInput(0));
}


template <class TInputImage>
const typename StreamingImageVirtualWriter<TInputImage>::InputImageType *
StreamingImageVirtualWriter<TInputImage>
::GetInput(unsigned int idx)
{
  return static_cast<TInputImage*> (this->itk::ProcessObject::GetInput(idx));
}
template <class TInputImage>
void
StreamingImageVirtualWriter<TInputImage>
::GenerateInputRequestedRegion(void)
{
  InputImagePointer inputPtr = const_cast< InputImageType * >( this->GetInput(0) );
  typename InputImageRegionType::SizeType size;
  typename InputImageRegionType::IndexType index;
  InputImageRegionType region;
  index.Fill(0);
  size.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);
  inputPtr->SetRequestedRegion(region);  
}
/**
 *
 */
template <class TInputImage>
unsigned long 
StreamingImageVirtualWriter<TInputImage>
::GetNumberOfStreamDivisions(void)
{
  return(CalculateNumberOfStreamDivisions());
}
template<class TInputImage>
unsigned long 
StreamingImageVirtualWriter<TInputImage>
::CalculateNumberOfStreamDivisions(void)
{	
	return StreamingTraitsType
    ::CalculateNumberOfStreamDivisions(this->GetInput(),
				       this->GetInput()->GetLargestPossibleRegion(),
                                       m_RegionSplitter,
				       m_CalculationDivision,
				       m_NumberOfStreamDivisions,
				       m_BufferMemorySize,
				       m_BufferNumberOfLinesDivisions);
}
/**
 *
 */
template <class TInputImage>
std::string 
StreamingImageVirtualWriter<TInputImage>
::GetMethodUseToCalculateNumberOfStreamDivisions(void)
{
  return (StreamingTraitsType::GetMethodUseToCalculateNumberOfStreamDivisions(m_CalculationDivision));
}
/**
 *
 */
template <class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Number of stream divisions: " << m_NumberOfStreamDivisions
     << std::endl;
  if (m_RegionSplitter)
    {
    os << indent << "Region splitter:" << m_RegionSplitter << std::endl;
    }
  else
    {
    os << indent << "Region splitter: (none)" << std::endl;
    }
}
template<class TInputImage>
void 
StreamingImageVirtualWriter<TInputImage>
::GenerateData(void)
{
  /**
   * Prepare all the outputs. This may deallocate previous bulk data.
   */
  this->PrepareOutputs();
  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);
  this->m_Updating = true;
  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent( itk::StartEvent() );
  /**
   * Grab the input
   */
  InputImagePointer inputPtr = const_cast< InputImageType * >( this->GetInput(0) );
  InputImageRegionType outputRegion = inputPtr->GetLargestPossibleRegion();
  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfStreamDivisions()
   * and what the Splitter thinks is a reasonable value.
   */
  unsigned int numDivisions;
  numDivisions = static_cast<unsigned int>(CalculateNumberOfStreamDivisions());

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;
  unsigned int piece;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
      streamRegion = m_RegionSplitter->GetSplit(piece, numDivisions,outputRegion);
      inputPtr->ReleaseData();
      inputPtr->SetRequestedRegion(streamRegion);
      inputPtr->Update();
      this->UpdateProgress((float) piece / numDivisions );
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
  this->InvokeEvent( itk::EndEvent() );

  /**
   * Now we have to mark the data as up to data.
   */
  for (unsigned int idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
    {
    if (this->GetOutput(idx))
      {
      this->GetOutput(idx)->DataHasBeenGenerated();
      }
    }
  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();
}
} // end namespace otb

#endif
