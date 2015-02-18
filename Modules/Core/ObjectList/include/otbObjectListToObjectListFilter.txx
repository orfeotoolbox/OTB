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
#ifndef __otbObjectListToObjectListFilter_txx
#define __otbObjectListToObjectListFilter_txx

#include "otbObjectListToObjectListFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
   * Constructor
 */
template <class TInputList, class TOutputList>
ObjectListToObjectListFilter<TInputList, TOutputList>
::ObjectListToObjectListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputList, class TOutputList>
void
ObjectListToObjectListFilter<TInputList, TOutputList>
::SetInput(const InputListType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputListType *>(input));
}

template <class TInputList, class TOutputList>
const typename ObjectListToObjectListFilter<TInputList, TOutputList>::InputListType *
ObjectListToObjectListFilter<TInputList, TOutputList>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputList *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TInputList, class TOutputList>
int
ObjectListToObjectListFilter<TInputList, TOutputList>
::SplitRequestedRegion(itk::ThreadIdType threadId,
                       int threadCount,
                       unsigned int requestedElements,
                       unsigned int& startIndex,
                       unsigned int& stopIndex)
{
  startIndex = static_cast<unsigned int>(vcl_floor(
                                           requestedElements * static_cast<double>(threadId) /
                                           static_cast<double>(threadCount) + 0.5
                                           ));
  stopIndex = static_cast<unsigned int>(vcl_floor(
                                          requestedElements *
                                          static_cast<double>(threadId + 1) / static_cast<double>(threadCount) + 0.5
                                          ));
  if (stopIndex > requestedElements) stopIndex = requestedElements;

  //Note: check the itkImageSource.txx for the compuration done there.
  // for now, there is no requested region for ObjectListFilter, so we don't
  // compute anything here.
  return threadCount;
}

/**
 * GenerateData
 */
template <class TInputList, class TOutputList>
void
ObjectListToObjectListFilter<TInputList, TOutputList>
::GenerateData(void)
{
  // Call a method that can be overridden by a subclass to perform
  // some calculations prior to splitting the main computations into
  // separate threads
  this->BeforeThreadedGenerateData();

  // Set up the multithreaded processing
  ThreadStruct str;
  str.Filter = this;

  // Initializing object per thread
  OutputListPointer defaultList;
  this->m_ObjectListPerThread = OutputListForThreadType(this->GetNumberOfThreads(), defaultList);

  // Setting up multithreader
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateData();

}

template <class TInputList, class TOutputList>
void
ObjectListToObjectListFilter<TInputList, TOutputList>
::BeforeThreadedGenerateData(void)
{
  this->AllocateOutputs();
}

template <class TInputList, class TOutputList>
void
ObjectListToObjectListFilter<TInputList, TOutputList>
::ThreadedGenerateData(unsigned int /*startIndex*/, unsigned int /*stopIndex*/, itk::ThreadIdType /*threadId*/)
{
  // The following code is equivalent to:
  // itkExceptionMacro("subclass should override this method!!!");
  // The ExceptionMacro is not used because gcc warns that a
  // 'noreturn' function does return
  std::ostringstream message;
  message << "itk::ERROR: " << this->GetNameOfClass()
          << "(" << this << "): " << "Subclass should override this method!!!";
  itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION);
  throw e_;

}

template <class TInputList, class TOutputList>
ITK_THREAD_RETURN_TYPE
ObjectListToObjectListFilter<TInputList, TOutputList>
::ThreaderCallback(void *arg)
{
  ThreadStruct *str;
  itk::ThreadIdType threadId, threadCount;
  unsigned int  total, start, stop;
  unsigned int  requestedElements;

  threadId = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->ThreadID;
  threadCount = ((itk::MultiThreader::ThreadInfoStruct *) (arg))->NumberOfThreads;
  str = (ThreadStruct *) (((itk::MultiThreader::ThreadInfoStruct *) (arg))->UserData);

  requestedElements = str->Filter->GetInput()->Size();
  total = str->Filter->SplitRequestedRegion(threadId, threadCount, requestedElements, start, stop);

  if (threadId < static_cast<itk::ThreadIdType>(total))
    {

    // For very small list it might occur that start = stop. In this
    // case the vertex at that index will be processed in the next strip.
    if (start != stop)
      {
      str->Filter->ThreadedGenerateData(start, stop, threadId);
      }
    }
  // else
  //   {
  //   otherwise don't use this thread. Sometimes the threads dont
  //   break up very well and it is just as efficient to leave a
  //   few threads idle.
  //   }

  return ITK_THREAD_RETURN_VALUE;
}

/**
   * PrintSelf Method
 */
template <class TInputList, class TOutputList>
void
ObjectListToObjectListFilter<TInputList, TOutputList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
