/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbUnaryFunctorObjectListFilter_hxx
#define otbUnaryFunctorObjectListFilter_hxx

#include "otbUnaryFunctorObjectListFilter.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputList, class TOutputList, class TFunction>
UnaryFunctorObjectListFilter<TInputList, TOutputList, TFunction>::UnaryFunctorObjectListFilter()
{
}

template <class TInputList, class TOutputList, class TFunction>
void UnaryFunctorObjectListFilter<TInputList, TOutputList, TFunction>::ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex,
                                                                                            itk::ThreadIdType threadId)
{

  InputListPointer inputPtr             = this->GetInput();
  this->m_ObjectListPerThread[threadId] = OutputListType::New();

  itk::ProgressReporter progress(this, threadId, stopIndex - startIndex);

  // Define the iterators
  InputListIterator it    = inputPtr->Begin();
  unsigned int      count = 0;
  while ((count < startIndex) && (it != inputPtr->End()))
  {
    ++it;
    ++count;
  }

  while ((count < stopIndex) && (it != inputPtr->End()))
  {
    this->m_ObjectListPerThread[threadId]->PushBack(m_Functor(it.Get()));

    progress.CompletedPixel();
    ++it;
    ++count;
  }
}

template <class TInputList, class TOutputList, class TFunction>
void UnaryFunctorObjectListFilter<TInputList, TOutputList, TFunction>::AfterThreadedGenerateData()
{
  // copy the lists to the output
  OutputListPointer outputPtr = this->GetOutput();
  for (unsigned int i = 0; i < this->m_ObjectListPerThread.size(); ++i)
  {
    if (this->m_ObjectListPerThread[i].IsNotNull())
    {
      for (OutputListIterator it = this->m_ObjectListPerThread[i]->Begin(); it != this->m_ObjectListPerThread[i]->End(); ++it)
      {
        outputPtr->PushBack(it.Get());
      }
    }
  }
}

} // end namespace otb

#endif
