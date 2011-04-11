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

#include "otbMacro.h"
#include "otbConfigure.h"
#include "itkCommand.h"

#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"

namespace otb
{

template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>
::StreamingImageVirtualWriter()
{
  // By default, we use tiled streaming, with automatic tile size
  // We don't set any parameter, so the memory size is retrieved rom the OTB configuration options
  m_StreamingManager = otb::RAMDrivenTiledStreamingManager<TInputImage>::New();
}

template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>
::~StreamingImageVirtualWriter()
{
}

template <class TInputImage>
void
StreamingImageVirtualWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage>
void
StreamingImageVirtualWriter<TInputImage>
::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<TInputImage> NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void
StreamingImageVirtualWriter<TInputImage>
::GenerateInputRequestedRegion(void)
{
  InputImagePointer                        inputPtr = const_cast<InputImageType *>(this->GetInput(0));

  InputImageRegionType                     region;
  typename InputImageRegionType::SizeType  size;
  typename InputImageRegionType::IndexType index;

  index.Fill(0);
  size.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);
  inputPtr->SetRequestedRegion(region);
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
  this->InvokeEvent(itk::StartEvent());

  /**
   * Grab the input
   */
  InputImagePointer    inputPtr = const_cast<InputImageType *>(this->GetInput(0));
  InputImageRegionType outputRegion = inputPtr->GetLargestPossibleRegion();
  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfStreamDivisions()
   * and what the Splitter thinks is a reasonable value.
   */
  m_StreamingManager->PrepareStreaming(inputPtr, outputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  /**
   * Register to the ProgressEvent of the source filter
   */
  // Get the source process object
  itk::ProcessObject* source = inputPtr->GetSource();

  // Check if source exists
  if(source)
    {
    typedef itk::MemberCommand<Self> CommandType;
    typedef typename CommandType::Pointer CommandPointerType;

    CommandPointerType command = CommandType::New();
    command->SetCallbackFunction(this, &Self::ObserveSourceFilterProgress);

    source->AddObserver(itk::ProgressEvent(), command);
    }
  else
    {
    itkWarningMacro(<< "Could not get the source process object. Progress report might be buggy");
    }

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;
  for (m_CurrentDivision = 0;
       m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
       m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
    {
    streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);
    otbMsgDevMacro(<< "Processing region : " << streamRegion )
    inputPtr->ReleaseData();
    inputPtr->SetRequestedRegion(streamRegion);
    inputPtr->Update();
    }

  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if (!this->GetAbortGenerateData())
    {
    this->UpdateProgress(1.0);
    }

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent());

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
