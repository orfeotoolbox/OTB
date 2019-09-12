/*
 * Copyright (C) 2017-2019 CS Systemes d'Information (CS SI)
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

#include "otbMultiImageFileWriter.h"
#include "otbImageIOFactory.h"

namespace otb
{

MultiImageFileWriter
::MultiImageFileWriter() :
 m_NumberOfDivisions(0),
 m_CurrentDivision(0),
 m_DivisionProgress(0.0),
 m_IsObserving(true),
 m_ObserverID(0)
{
  // By default, we use tiled streaming, with automatic tile size
  // We don't set any parameter, so the memory size is retrieved from the OTB configuration options
  this->SetAutomaticAdaptativeStreaming();
  // add a fake output to drive memory estimation
  this->SetNthOutput(0, FakeOutputType::New());
}

void
MultiImageFileWriter
::SetNumberOfDivisionsStrippedStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsStrippedStreamingManager<FakeOutputType> NumberOfDivisionsStrippedStreamingManagerType;
  NumberOfDivisionsStrippedStreamingManagerType::Pointer streamingManager =
    NumberOfDivisionsStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetNumberOfDivisionsTiledStreaming(unsigned int nbDivisions)
{
  typedef NumberOfDivisionsTiledStreamingManager<FakeOutputType> NumberOfDivisionsTiledStreamingManagerType;
  NumberOfDivisionsTiledStreamingManagerType::Pointer streamingManager =
    NumberOfDivisionsTiledStreamingManagerType::New();
  streamingManager->SetNumberOfDivisions(nbDivisions);

  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetNumberOfLinesStrippedStreaming(unsigned int nbLinesPerStrip)
{
  typedef NumberOfLinesStrippedStreamingManager<FakeOutputType> NumberOfLinesStrippedStreamingManagerType;
  NumberOfLinesStrippedStreamingManagerType::Pointer streamingManager =
    NumberOfLinesStrippedStreamingManagerType::New();
  streamingManager->SetNumberOfLinesPerStrip(nbLinesPerStrip);

  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetAutomaticStrippedStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenStrippedStreamingManager<FakeOutputType> RAMDrivenStrippedStreamingManagerType;
  RAMDrivenStrippedStreamingManagerType::Pointer streamingManager =
    RAMDrivenStrippedStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);

  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetTileDimensionTiledStreaming(unsigned int tileDimension)
{
  typedef TileDimensionTiledStreamingManager<FakeOutputType> TileDimensionTiledStreamingManagerType;
  TileDimensionTiledStreamingManagerType::Pointer streamingManager =
    TileDimensionTiledStreamingManagerType::New();
  streamingManager->SetTileDimension(tileDimension);

  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetAutomaticTiledStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenTiledStreamingManager<FakeOutputType> RAMDrivenTiledStreamingManagerType;
  RAMDrivenTiledStreamingManagerType::Pointer streamingManager =
    RAMDrivenTiledStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::SetAutomaticAdaptativeStreaming(unsigned int availableRAM, double bias)
{
  typedef RAMDrivenAdaptativeStreamingManager<FakeOutputType> RAMDrivenAdaptativeStreamingManagerType;
  RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager =
    RAMDrivenAdaptativeStreamingManagerType::New();
  streamingManager->SetAvailableRAMInMB(availableRAM);
  streamingManager->SetBias(bias);
  m_StreamingManager = streamingManager;
}

void
MultiImageFileWriter
::InitializeStreaming()
{
//  const ImageBaseType* inputPtr = this->GetInput(0);
  if(m_SinkList.size() == 0)
    itkExceptionMacro("At least one input must be connected to the writer\n");
  const ImageBaseType* inputPtr = m_SinkList[0]->GetInput();
  if(!inputPtr)
    itkExceptionMacro("At least one input must be connected to the writer\n");

  /** Control if the ImageIO is CanStreamWrite */
  m_NumberOfDivisions = 1;
  bool canStream = true;
  bool isBuffered = true;
  for (unsigned int inputIndex = 0; inputIndex < m_SinkList.size(); ++inputIndex)
    {
    if (!m_SinkList[inputIndex]->CanStreamWrite())
      {
      canStream = false;
      }
    if (m_SinkList[inputIndex]->GetInput()->GetBufferedRegion() !=
        m_SinkList[inputIndex]->GetInput()->GetLargestPossibleRegion())
      {
      isBuffered = false;
      }
    }
  if (canStream == false)
    {
    otbWarningMacro(
      << "One of the selected ImageIO does not support streaming.");
    this->SetNumberOfDivisionsStrippedStreaming(m_NumberOfDivisions);
    }

  /** Compare the buffered region  with the inputRegion which is the largest
  * possible region or a user defined region through extended filename
  * Not sure that if this modification is needed  */
  else if (isBuffered)
    {
    otbMsgDevMacro(<< "Buffered region is the largest possible region, there is"
      " no need for streaming.");
    this->SetNumberOfDivisionsStrippedStreaming(m_NumberOfDivisions);
    }
  else
    {
    /**
     * Determine of number of pieces to divide the input.  This will be the
     * first estimated on the fake output, which has the same size as the
     * first input. Then there is a check that each input can be split into
     * this number of pieces.
     */
    FakeOutputType * fakeOut = static_cast<FakeOutputType *>(
      this->itk::ProcessObject::GetOutput(0));
    RegionType region = fakeOut->GetLargestPossibleRegion();
    m_StreamingManager->PrepareStreaming(fakeOut, region);
    m_NumberOfDivisions = m_StreamingManager->GetNumberOfSplits();
    // Check this number of division is compatible with all inputs
    bool nbDivValid = false;
    while ( (!nbDivValid) && 1 < m_NumberOfDivisions)
      {
      unsigned int smallestNbDiv = m_NumberOfDivisions;
      for (unsigned int i = 0; i < m_SinkList.size(); ++i)
        {
        unsigned int div = m_StreamingManager->GetSplitter()->GetNumberOfSplits(
          m_SinkList[i]->GetInput()->GetLargestPossibleRegion(),
          m_NumberOfDivisions);
        smallestNbDiv = std::min(div, smallestNbDiv);
        }
      if (smallestNbDiv == m_NumberOfDivisions)
        {
        nbDivValid = true;
        }
      else
        {
        m_NumberOfDivisions = smallestNbDiv;
        }
      }
    if (m_NumberOfDivisions == 1)
      {
      otbWarningMacro("Can't find a common split scheme between all inputs, streaming disabled\n");
      }
    otbMsgDebugMacro(<< "Number Of Stream Divisions : " << m_NumberOfDivisions);
    }
}

void
MultiImageFileWriter
::ResetAllRequestedRegions(ImageBaseType* imagePtr)
{
  RegionType nullRegion = imagePtr->GetLargestPossibleRegion();
  nullRegion.SetSize(0, 0);
  nullRegion.SetSize(1, 0);

  imagePtr->SetRequestedRegion(nullRegion);
  if(imagePtr->GetSource())
    {
    itk::ProcessObject::DataObjectPointerArray inputs = imagePtr->GetSource()->GetInputs();
    for( itk::ProcessObject::DataObjectPointerArray::iterator
        it = inputs.begin();
        it != inputs.end();
        ++it )
      {
      ImageBaseType * inputImagePtr = dynamic_cast<ImageBaseType*>(it->GetPointer());
      if(inputImagePtr != NULL)
        {
        ResetAllRequestedRegions(inputImagePtr);
        }
      }
    }
}

void
MultiImageFileWriter
::UpdateOutputInformation()
{
  for(unsigned int inputIndex = 0; inputIndex < m_SinkList.size(); ++inputIndex)
    {
    m_SinkList[inputIndex]->WriteImageInformation();
    }
  this->GenerateOutputInformation();
}

void
MultiImageFileWriter
::UpdateOutputData(itk::DataObject * itkNotUsed(output))
{
  /**
   * prevent chasing our tail
   */
  if (this->m_Updating)
    {
    return;
    }

  // Initialize streaming
  this->InitializeStreaming();

  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);
  this->m_Updating = true;

  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent(itk::StartEvent());

  this->UpdateProgress(0);
  m_CurrentDivision = 0;
  m_DivisionProgress = 0;

  /** Loop over the number of pieces, set and propagate requested regions then
   * update pipeline upstream
   */
  int numInputs = m_SinkList.size(); //this->GetNumberOfInputs();
  m_StreamRegionList.resize(numInputs);
  itkDebugMacro( "Number of streaming divisions: " << m_NumberOfDivisions);

  // Add observer only to first input
  if(numInputs > 0)
    {
    m_IsObserving = false;
    m_ObserverID = 0;

    typedef itk::MemberCommand<Self> CommandType;
    typedef CommandType::Pointer CommandPointerType;

    CommandPointerType command = CommandType::New();
    command->SetCallbackFunction(this, &Self::ObserveSourceFilterProgress);

    itk::ProcessObject* src = this->GetInput(0)->GetSource();
    m_ObserverID = src->AddObserver(itk::ProgressEvent(), command);
    m_IsObserving = true;
    }

  for (m_CurrentDivision = 0; m_CurrentDivision < m_NumberOfDivisions && !this->GetAbortGenerateData();
      m_CurrentDivision++, m_DivisionProgress = 0, this->UpdateFilterProgress())
    {
    // Update all stream regions
    for(int inputIndex = 0; inputIndex < numInputs; ++inputIndex)
      {
      m_StreamRegionList[inputIndex] = GetStreamRegion(inputIndex);
      }

    // NOTE : this reset was probably designed to work with the next section
    // Where the final requested region is the "union" between the computed
    // requested region and the current requested region.
    
    // Reset requested regions for all images
    for(int inputIndex = 0; inputIndex < numInputs; ++inputIndex)
      {
      ResetAllRequestedRegions(m_SinkList[inputIndex]->GetInput());
      }

    for(int inputIndex = 0; inputIndex < numInputs; ++inputIndex)
      {
      ImageBaseType::Pointer inputPtr = m_SinkList[inputIndex]->GetInput();
      RegionType inputRequestedRegion = m_StreamRegionList[inputIndex];
      const RegionType & currentInputRequestedRegion = inputPtr->GetRequestedRegion();
      if( currentInputRequestedRegion != inputPtr->GetLargestPossibleRegion()
        && currentInputRequestedRegion.GetNumberOfPixels() != 0)
        {
        IndexType startIndex = currentInputRequestedRegion.GetIndex();
        IndexType lastIndex = currentInputRequestedRegion.GetUpperIndex();
        startIndex[0] = std::min(startIndex[0], inputRequestedRegion.GetIndex(0));
        startIndex[1] = std::min(startIndex[1], inputRequestedRegion.GetIndex(1));
        lastIndex[0] = std::max(lastIndex[0], inputRequestedRegion.GetUpperIndex()[0]);
        lastIndex[1] = std::max(lastIndex[1], inputRequestedRegion.GetUpperIndex()[1]);
        inputRequestedRegion.SetIndex(startIndex);
        inputRequestedRegion.SetUpperIndex(lastIndex);
        }

      inputPtr->SetRequestedRegion(inputRequestedRegion);
      inputPtr->PropagateRequestedRegion();
      }

    /** Call GenerateData to write streams to files if needed */
    this->GenerateData();
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

  if (m_IsObserving)
    {
    ImageBaseType::Pointer inputPtr = m_SinkList[0]->GetInput(); // const_cast<ImageBaseType *>(this->GetInput(0));
    itk::ProcessObject* source = inputPtr->GetSource();
    m_IsObserving = false;
    source->RemoveObserver(m_ObserverID);
    }

  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();

  // Mark that we are no longer updating the data in this filter
  this->m_Updating = false;
}


void
MultiImageFileWriter
::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();

  // Approximate conversion of output requested region into each input,
  // but this is only to have a consistent pipeline memory estimation.
  int numInputs = m_SinkList.size(); //this->GetNumberOfInputs();

  FakeOutputType* fakeOut = static_cast<FakeOutputType *>(
    this->itk::ProcessObject::GetOutput(0));

  if (numInputs)
    {
    RegionType refLargest = fakeOut->GetLargestPossibleRegion();
    RegionType refRequest = fakeOut->GetRequestedRegion();
    IndexType idxLargest = refLargest.GetIndex();
    SizeType sizeLargest = refLargest.GetSize();
    IndexType idxRequest = refRequest.GetIndex();
    SizeType sizeRequest = refRequest.GetSize();
    for (int i = 0; i < numInputs; ++i)
      {
      ImageBaseType* inputPtr = m_SinkList[i]->GetInput();
      if(!inputPtr)
        {
        return;
        }
      RegionType region = inputPtr->GetLargestPossibleRegion();
      IndexType idx = region.GetIndex();
      SizeType size = region.GetSize();
      idx[0] += size[0] * (idxRequest[0] - idxLargest[0]) / sizeLargest[0];
      idx[1] += size[1] * (idxRequest[1] - idxLargest[1]) / sizeLargest[1];
      size[0] *= sizeRequest[0] / sizeLargest[0];
      size[1] *= sizeRequest[1] / sizeLargest[1];
      region.SetIndex(idx);
      region.SetSize(size);
      inputPtr->SetRequestedRegion(region);
      }
    }
}

void
MultiImageFileWriter
::GenerateData()
{
  int numInputs = m_SinkList.size();
  for(int inputIndex = 0; inputIndex < numInputs; ++inputIndex)
    {
    m_SinkList[inputIndex]->Write(m_StreamRegionList[inputIndex]);
    }
}

MultiImageFileWriter::RegionType
MultiImageFileWriter
::GetStreamRegion(int inputIndex)
{
  const SinkBase::Pointer sink = m_SinkList[inputIndex];
  RegionType region = sink->GetInput()->GetLargestPossibleRegion();

  m_StreamingManager->GetSplitter()->GetSplit(
    m_CurrentDivision,
    m_NumberOfDivisions,
    region);
  return region;
}

} // end of namespace otb
