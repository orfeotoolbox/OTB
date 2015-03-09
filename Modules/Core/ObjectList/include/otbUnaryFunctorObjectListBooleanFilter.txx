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
#ifndef __otbUnaryFunctorObjectListBooleanFilter_txx
#define __otbUnaryFunctorObjectListBooleanFilter_txx

#include "otbUnaryFunctorObjectListBooleanFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputList, class TOutputList, class TFunction>
UnaryFunctorObjectListBooleanFilter<TInputList, TOutputList, TFunction>
::UnaryFunctorObjectListBooleanFilter()
{
}

template <class TInputList, class TOutputList, class TFunction>
void
UnaryFunctorObjectListBooleanFilter<TInputList, TOutputList, TFunction>
::ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex, itk::ThreadIdType threadId)
{

  InputListPointer inputPtr = this->GetInput();
  this->m_ObjectListPerThread[threadId] = OutputListType::New();

  itk::ProgressReporter progress(this, threadId, stopIndex - startIndex);

  // Define the iterators
  InputListIterator it = inputPtr->Begin();
  unsigned int      count = 0;
  while ((count < startIndex) && (it != inputPtr->End()))
    {
    ++it;
    ++count;
    }

  while ((count < stopIndex) && (it != inputPtr->End()))
    {
    if (m_Functor(it.Get()))
      {
      this->m_ObjectListPerThread[threadId]->PushBack(it.Get());
      }

    progress.CompletedPixel();
    ++it;
    ++count;
    }

}

template <class TInputList, class TOutputList, class TFunction>
void
UnaryFunctorObjectListBooleanFilter<TInputList, TOutputList, TFunction>
::AfterThreadedGenerateData()
{
  // copy the lists to the output
  OutputListPointer outputPtr = this->GetOutput();
  for (unsigned int i = 0; i < this->m_ObjectListPerThread.size(); ++i)
    {
    if (this->m_ObjectListPerThread[i].IsNotNull())
      {
      for (OutputListIterator it = this->m_ObjectListPerThread[i]->Begin();
           it != this->m_ObjectListPerThread[i]->End();
           ++it)
        {
        outputPtr->PushBack(it.Get());
        }
      }
    }

}

} // end namespace otb

#endif
