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

#ifndef otbListSampleToListSample_hxx
#define otbListSampleToListSample_hxx

#include "otbListSampleToListSampleFilter.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::ListSampleToListSampleFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::SetInput( const InputSampleListType * inputPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputSampleListType* >( inputPtr ) );
}

template < class TInputSampleList, class TOutputSampleList >
const typename ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::InputSampleListType *
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const InputSampleListType * >
    (this->itk::ProcessObject::GetInput(0) );
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
