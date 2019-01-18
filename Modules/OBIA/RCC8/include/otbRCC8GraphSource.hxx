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

#ifndef otbRCC8GraphSource_hxx
#define otbRCC8GraphSource_hxx

#include "otbRCC8GraphSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputGraph>
RCC8GraphSource<TOutputGraph>
::RCC8GraphSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, TOutputGraph::New().GetPointer());
}
/**
 * Get the output Graph
 * \return The output graph produced.
 */
template <class TOutputGraph>
typename RCC8GraphSource<TOutputGraph>::OutputGraphType *
RCC8GraphSource<TOutputGraph>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return nullptr;
    }
  return static_cast<OutputGraphType *> (this->ProcessObject::GetOutput(0));
}
/**
 * PrintSelf Method
 */
template<class TOutputGraph>
void
RCC8GraphSource<TOutputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
