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
#ifndef __otbUnaryFunctorObjectListFilter_txx
#define __otbUnaryFunctorObjectListFilter_txx

#include "otbUnaryFunctorObjectListFilter.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputList, class TOutputList, class TFunction  >
UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
::UnaryFunctorObjectListFilter()
{
}



/**
 * GenerateData apply the functor on each element of the list
 */
template <class TInputList, class TOutputList, class TFunction  >
void
UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
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
  m_ObjectListPerThread = OutputListForThreadType(this->GetNumberOfThreads(),defaultList);


  // Setting up multithreader
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateData();

}



template <class TInputList, class TOutputList, class TFunction  >
    void
        UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
  ::BeforeThreadedGenerateData()
{

  this->AllocateOutputs();

}

template <class TInputList, class TOutputList, class TFunction  >
void
UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
::ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex,int threadId)
{

  InputListPointer inputPtr = this->GetInput();
  m_ObjectListPerThread[threadId] = OutputListType::New();

  itk::ProgressReporter progress(this, threadId, stopIndex-startIndex);

  // Define the iterators
  InputListIterator it = inputPtr->Begin();
  unsigned int count = 0;
  while ((count < startIndex) && (it != inputPtr->End()))
  {
    ++it;
    ++count;
  }

  while((count < stopIndex) && (it != inputPtr->End()))
  {
    m_ObjectListPerThread[threadId]->PushBack(m_Functor(it.Get()));

    progress.CompletedPixel();
    ++it;
    ++count;
  }

}


template <class TInputList, class TOutputList, class TFunction  >
    void
        UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
  ::AfterThreadedGenerateData()
{
  // copy the lists to the output
  OutputListPointer outputPtr = this->GetOutput();
  for (int i=0; i< m_ObjectListPerThread.size(); ++i)
  {
    for(OutputListIterator it = m_ObjectListPerThread[i]->Begin();
        it != m_ObjectListPerThread[i]->End();
        ++it)
    {
      outputPtr->PushBack(it.Get());
    }
  }

}



template <class TInputList, class TOutputList, class TFunction  >
    ITK_THREAD_RETURN_TYPE
        UnaryFunctorObjectListFilter<TInputList,TOutputList,TFunction>
  ::ThreaderCallback( void *arg )
{
  ThreadStruct *str;
  int threadId, threadCount;
  unsigned int total,start, stop;

  threadId = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  threadCount = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;
  str = (ThreadStruct *)(((itk::MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  total = str->Filter->GetInput()->Size();

  if (threadId < static_cast<int>(total))
  {

    // Split the adjacency matrix in strip of equal dimension
    start = static_cast<unsigned int>( vcl_floor(
                          total*static_cast<double>(threadId)/static_cast<double>(threadCount)+0.5
                         ));
    stop = static_cast<unsigned int>(vcl_floor(
                          total*static_cast<double>(threadId+1)/static_cast<double>(threadCount)+0.5
                         ));
    if (stop > total)
      stop = total;

    // For very small list it might occur that start = stop. In this
    // case the vertex at that index will be processed in the next strip.
    if(start!=stop)
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



} // end namespace otb

#endif
