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

#ifndef otbListSampleSource_hxx
#define otbListSampleSource_hxx

#include "otbListSampleSource.h"

namespace otb
{
namespace Statistics
{

template <class TInputSampleList, class TOutputSampleList>
ListSampleSource<TInputSampleList, TOutputSampleList>::ListSampleSource()
{
  this->SetNumberOfRequiredOutputs(1);

  // Generate the output sample list
  typename OutputSampleListType::Pointer outputPtr = static_cast<OutputSampleListType*>(this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());
}

template <class TInputSampleList, class TOutputSampleList>
typename ListSampleSource<TInputSampleList, TOutputSampleList>::DataObjectPointer
    ListSampleSource<TInputSampleList, TOutputSampleList>::MakeOutput(DataObjectPointerArraySizeType)
{
  OutputSampleListPointer outputSampleList = OutputSampleListType::New();
  return static_cast<DataObjectPointer>(outputSampleList);
}

template <class TInputSampleList, class TOutputSampleList>
typename ListSampleSource<TInputSampleList, TOutputSampleList>::OutputSampleListType* ListSampleSource<TInputSampleList, TOutputSampleList>::GetOutput()
{
  return static_cast<OutputSampleListType*>(this->ProcessObject::GetOutput(0));
}

template <class TInputSampleList, class TOutputSampleList>
void ListSampleSource<TInputSampleList, TOutputSampleList>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
