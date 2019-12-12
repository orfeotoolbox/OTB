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


#ifndef otbLabelMapSource_hxx
#define otbLabelMapSource_hxx

#include "otbLabelMapSource.h"

namespace otb
{

template <class TOutputLabelMap>
LabelMapSource<TOutputLabelMap>::LabelMapSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, OutputLabelMapType::New().GetPointer());
}

template <class TOutputLabelMap>
LabelMapSource<TOutputLabelMap>::~LabelMapSource()
{
}

template <class TOutputLabelMap>
void LabelMapSource<TOutputLabelMap>::AllocateOutputs()
{
  OutputLabelMapPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    outputPtr = this->GetOutput(i);

    // TODO Need to implement the Clear Method in the class itk::LabelMap
    // outputPtr->Clear();
  }
}

template <class TOutputLabelMap>
typename LabelMapSource<TOutputLabelMap>::OutputLabelMapType* LabelMapSource<TOutputLabelMap>::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }
  return static_cast<OutputLabelMapType*>(this->ProcessObject::GetOutput(0));
}

template <class TOutputLabelMap>
typename LabelMapSource<TOutputLabelMap>::OutputLabelMapType* LabelMapSource<TOutputLabelMap>::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<OutputLabelMapType*>(this->Superclass::GetOutput(idx));
}

template <class TOutputLabelMap>
void LabelMapSource<TOutputLabelMap>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
