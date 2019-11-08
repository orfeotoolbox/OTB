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

#ifndef otbStreamingImageVirtualWriter_hxx
#define otbStreamingImageVirtualWriter_hxx
#include "otbStreamingImageVirtualWriter.h"

#include "otbMacro.h"
#include "itkCommand.h"

#include "otbNumberOfDivisionsStrippedStreamingManager.h"
#include "otbNumberOfDivisionsTiledStreamingManager.h"
#include "otbNumberOfLinesStrippedStreamingManager.h"
#include "otbRAMDrivenStrippedStreamingManager.h"
#include "otbTileDimensionTiledStreamingManager.h"
#include "otbRAMDrivenTiledStreamingManager.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"
#include "otbUtils.h"

namespace otb
{

template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>::StreamingImageVirtualWriter()
  : m_NumberOfDivisions(0), m_CurrentDivision(0), m_DivisionProgress(0.0), m_IsObserving(true), m_ObserverID(0)
{
  // By default, we use tiled streaming, with automatic tile size
  // We don't set any parameter, so the memory size is retrieved from the OTB configuration options
  this->SetAutomaticAdaptativeStreaming();
}

template <class TInputImage>
StreamingImageVirtualWriter<TInputImage>::~StreamingImageVirtualWriter()
{
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsStrippedStreamingManager<TInputImage>  NumberOfDivisionsStrippedStreamingManagerType;
  typename NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager = NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsTiledStreamingManager<TInputImage>  NumberOfDivisionsTiledStreamingManagerType;
  typename NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager = NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<TInputImage>  NumberOfLinesStrippedStreamingManagerType;
  typename NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager = NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenStrippedStreamingManager<TInputImage>  RAMDrivenStrippedStreamingManagerType;
  typename RAMDrivenStrippedStreamingManagerType::Pointer streamingManager = RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetTileDimensionTiledStreaming(unsigned int tileDimension)
{
  typedef TileDimensionTiledStreamingManager<TInputImage>  TileDimensionTiledStreamingManagerType;
  typename TileDimensionTiledStreamingManagerType::Pointer streamingManager = TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);

  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenTiledStreamingManager<TInputImage>  RAMDrivenTiledStreamingManagerType;
  typename RAMDrivenTiledStreamingManagerType::Pointer streamingManager = RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetAutomaticAdaptativeStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenAdaptativeStreamingManager<TInputImage>  RAMDrivenAdaptativeStreamingManagerType;
  typename RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::Update()
{
  InputImagePointer inputPtr = const_cast<InputImageType*>(this->GetInput(0));
  inputPtr->UpdateOutputInformation();

  this->GenerateData();
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::GenerateInputRequestedRegion(void)
{
  InputImagePointer inputPtr = const_cast<InputImageType*>(this->GetInput(0));

  InputImageRegionType                     region;
  typename InputImageRegionType::SizeType  size;
  typename InputImageRegionType::IndexType index;

  index.Fill(0);
  size.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);
  inputPtr->SetRequestedRegion(region);
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::GenerateData(void)
{
  otb::Logger::Instance()->LogSetupInformation();

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
  InputImagePointer    inputPtr     = const_cast<InputImageType*>(this->GetInput(0));
  InputImageRegionType outputRegion = inputPtr->GetLargestPossibleRegion();
  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfDivisionsStrippedStreaming()
   * and what the Splitter thinks is a reasonable value.
   */
  m_StreamingManager->PrepareStreaming(inputPtr, outputRegion);
  m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  /**
   * Register to the ProgressEvent of the source filter
   */
  // Get the source process object
  itk::ProcessObject* source = inputPtr->GetSource();
  m_IsObserving              = false;
  m_ObserverID               = 0;

  // Check if source exists
  if (source)
  {
    typedef itk::MemberCommand<Self>      CommandType;
    typedef typename CommandType::Pointer CommandPointerType;

    CommandPointerType command = CommandType::New();
    command->SetCallbackFunction(this, &Self::ObserveSourceFilterProgress);

    m_ObserverID  = source->AddObserver(itk::ProgressEvent(), command);
    m_IsObserving = true;
  }

  const auto firstSplitSize = m_StreamingManager->GetSplit(0).GetSize();
  otbLogMacro(Info, << "Estimation will be performed in " << m_NumberOfDivisions << " blocks of " << firstSplitSize[0] << "x" << firstSplitSize[1]
                    << " pixels");


  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;
  for (m_CurrentDivision = 0; m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
       m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
  {
    streamRegion = m_StreamingManager->GetSplit(m_CurrentDivision);
    // inputPtr->ReleaseData();
    // inputPtr->SetRequestedRegion(streamRegion);
    // inputPtr->Update();
    inputPtr->SetRequestedRegion(streamRegion);
    inputPtr->PropagateRequestedRegion();
    inputPtr->UpdateOutputData();
  }

  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if (!this->GetAbortGenerateData())
  {
    this->UpdateProgress(1.0);
  }
  else
  {
    itk::ProcessAborted e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Image streaming has been aborted");
    throw e;
  }

  // Notify end event observers
  this->InvokeEvent(itk::EndEvent());

  if (m_IsObserving)
  {
    m_IsObserving = false;
    source->RemoveObserver(m_ObserverID);
  }

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

template <class TInputImage>
const bool& StreamingImageVirtualWriter<TInputImage>::GetAbortGenerateData() const
{
  m_Lock.Lock();
  bool ret = Superclass::GetAbortGenerateData();
  m_Lock.Unlock();
  if (ret)
    return otb::Utils::TrueConstant;
  return otb::Utils::FalseConstant;
}

template <class TInputImage>
void StreamingImageVirtualWriter<TInputImage>::SetAbortGenerateData(bool val)
{
  m_Lock.Lock();
  Superclass::SetAbortGenerateData(val);
  m_Lock.Unlock();
}

} // end namespace otb

#endif
