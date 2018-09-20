/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToPointSetFilter_hxx
#define otbImageToPointSetFilter_hxx

#include "otbImageToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::ImageToPointSetFilter()
{
  this->ProcessObjectType::SetNumberOfRequiredInputs(1);

  OutputPointSetPointer output
    = dynamic_cast<OutputPointSetType*>(this->MakeOutput(0).GetPointer());

  ProcessObjectType::SetNumberOfRequiredOutputs(1);
  ProcessObjectType::SetNthOutput(0, output.GetPointer());

  m_PointsContainerPerThread.clear();
  m_PointDataContainerPerThread.clear();

  // create default region splitter
  m_StreamingManager = StreamingManagerType::New();
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::SetInput(unsigned int idx, const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(idx,
                                       const_cast<InputImageType *>(input));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::SetInput(const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(0,
                                       const_cast<InputImageType *>(input));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
const typename ImageToPointSetFilter<TInputImage, TOutputPointSet>::InputImageType *
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::GetInput(unsigned int idx)
{

  return dynamic_cast<const InputImageType*>
           (this->ProcessObjectType::GetInput(idx));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
const typename ImageToPointSetFilter<TInputImage, TOutputPointSet>::InputImageType *
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1) return nullptr;

  return dynamic_cast<const InputImageType*>
           (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::GenerateOutputInformation()
{
}

/**
 * GenerateData
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::GenerateData(void)
{

  PointsContainerType * outputPointsContainer = this->GetOutput()->GetPoints();
  outputPointsContainer->Initialize();

  PointDataContainerType * outputPointDataContainer = this->GetOutput()->GetPointData();
  outputPointDataContainer->Initialize();
  
  typename TInputImage::RegionType inputRegion = this->GetInput()->GetLargestPossibleRegion();

  // Input is an image, cast away the constness so we can set
  // the requested region.
  InputImagePointer input = const_cast<TInputImage *> (this->GetInput());

  m_StreamingManager->PrepareStreaming(input,inputRegion);

  unsigned long numDivisions = m_StreamingManager->GetNumberOfSplits();

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  unsigned int         piece;
  InputImageRegionType streamRegion;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
    streamRegion = m_StreamingManager->GetSplit(piece);
    typedef itk::ImageToImageFilterDetail::ImageRegionCopier<itkGetStaticConstMacro(InputImageDimension),
        itkGetStaticConstMacro(InputImageDimension)>
    OutputToInputRegionCopierType;
    OutputToInputRegionCopierType regionCopier;
    InputImageRegionType          inputRegion2;
    regionCopier(inputRegion2, streamRegion);
    input->SetRequestedRegion(inputRegion2);

    // Call a method that can be overridden by a subclass to perform
    // some calculations prior to splitting the main computations into
    // separate threads
    this->BeforeThreadedGenerateData();

    // Set up the multithreaded processing
    ThreadStruct str;
    str.Filter = this;

    // Initializing object per thread
    typename PointsContainerType::Pointer defaultPointsContainer = PointsContainerType::New();
    this->m_PointsContainerPerThread
      = OutputPointsContainerForThreadType(this->GetNumberOfThreads(), defaultPointsContainer);

    typename PointDataContainerType::Pointer defaultPointDataContainer = PointDataContainerType::New();
    this->m_PointDataContainerPerThread
      = OutputPointDataContainerForThreadType(this->GetNumberOfThreads(), defaultPointDataContainer);

    // Setting up multithreader
    this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
    this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

    // multithread the execution
    this->GetMultiThreader()->SingleMethodExecute();

    // Call a method that can be overridden by a subclass to perform
    // some calculations after all the threads have completed
    this->AfterThreadedGenerateData();
    }

}

template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::BeforeThreadedGenerateData(void)
{

}

template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::AfterThreadedGenerateData(void)
{
  // copy the lists to the output
  PointsContainerType * outputPointsContainer = this->GetOutput()->GetPoints();
  unsigned long currentIndex = outputPointsContainer->Size();

  typedef typename PointsContainerType::ConstIterator OutputPointsContainerIterator;
  for (unsigned int i = 0; i < this->m_PointsContainerPerThread.size(); ++i)
    {
    if (this->m_PointsContainerPerThread[i].IsNotNull())
      {
      for (OutputPointsContainerIterator it = this->m_PointsContainerPerThread[i]->Begin();
           it != this->m_PointsContainerPerThread[i]->End();
           ++it)
        {
        outputPointsContainer->InsertElement(currentIndex,it.Value());
        ++currentIndex;
        }
      }
    }

  PointDataContainerType * outputPointDataContainer = this->GetOutput()->GetPointData();
  currentIndex = outputPointDataContainer->Size();

  typedef typename PointDataContainerType::ConstIterator OutputPointDataContainerIterator;
  for (unsigned int i = 0; i < this->m_PointDataContainerPerThread.size(); ++i)
    {
    if (this->m_PointDataContainerPerThread[i].IsNotNull())
      {
      for (OutputPointDataContainerIterator it = this->m_PointDataContainerPerThread[i]->Begin();
           it != this->m_PointDataContainerPerThread[i]->End();
           ++it)
        {
        outputPointDataContainer->InsertElement(currentIndex,it.Value());
        ++currentIndex;
        }
      }
    }
}

template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::ThreadedGenerateData(const InputImageRegionType&, itk::ThreadIdType)
{
  // The following code is equivalent to:
  // itkExceptionMacro("subclass should override this method!!!");
  // The ExceptionMacro is not used because gcc warns that a
  // 'noreturn' function does return
  std::ostringstream message;
  message << "itk::ERROR: " << this->GetNameOfClass()
          << "(" << this << "): " << "Subclass should override this method!!!";
  itk::ExceptionObject e_(__FILE__, __LINE__, message.str(), ITK_LOCATION);
  throw e_;

}

template <class TInputImage, class TOutputPointSet>
ITK_THREAD_RETURN_TYPE
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::ThreaderCallback(void *arg)
{
  ThreadStruct           *str;
  unsigned int           total, threadCount;
  itk::ThreadIdType      threadId;

  threadId = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->ThreadID;
  threadCount = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->NumberOfThreads;
  str = (ThreadStruct *) (((itk::MultiThreader::ThreadInfoStruct *) (arg))->UserData);

  // execute the actual method with appropriate output region
  // first find out how many pieces extent can be split into.
  typename TInputImage::RegionType splitRegion;
  total = str->Filter->SplitRequestedRegion(threadId, threadCount,
                                            splitRegion);

  if (threadId < total)
    {
    str->Filter->ThreadedGenerateData(splitRegion, threadId);
    }
  // else
  //   {
  //   otherwise don't use this thread. Sometimes the threads don't
  //   break up very well and it is just as efficient to leave a
  //   few threads idle.
  //   }

  return ITK_THREAD_RETURN_VALUE;
}

template <class TInputImage, class TOutputPointSet>
int
ImageToPointSetFilter<TInputImage, TOutputPointSet>
::SplitRequestedRegion(int i, int num, InputImageRegionType& splitRegion)
{
  // Get the output pointer
  typename InputImageType::ConstPointer inputPtr = this->GetInput();
  const typename TInputImage::SizeType& requestedRegionSize
    = inputPtr->GetRequestedRegion().GetSize();

  int                             splitAxis;
  typename TInputImage::IndexType splitIndex;
  typename TInputImage::SizeType  splitSize;

  // Initialize the splitRegion to the output requested region
  splitRegion = inputPtr->GetRequestedRegion();
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  // split on the outermost dimension available
  splitAxis = inputPtr->GetImageDimension() - 1;
  while (requestedRegionSize[splitAxis] == 1)
    {
    --splitAxis;
    if (splitAxis < 0)
      { // cannot split
      itkDebugMacro("  Cannot Split");
      return 1;
      }
    }

  // determine the actual number of pieces that will be generated
  typename TInputImage::SizeType::SizeValueType range = requestedRegionSize[splitAxis];
  int                                           valuesPerThread = (int) ::std::ceil(range / (double) num);
  int                                           maxThreadIdUsed =
    (int) ::std::ceil(range / (double) valuesPerThread) - 1;

  // Split the region
  if (i < maxThreadIdUsed)
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    splitSize[splitAxis] = valuesPerThread;
    }
  if (i == maxThreadIdUsed)
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    // last thread needs to process the "rest" dimension being split
    splitSize[splitAxis] = splitSize[splitAxis] - i * valuesPerThread;
    }

  // set the split region ivars
  splitRegion.SetIndex(splitIndex);
  splitRegion.SetSize(splitSize);

  itkDebugMacro("  Split Piece: " << splitRegion);

  return maxThreadIdUsed + 1;
}

} // end namespace otb

#endif
