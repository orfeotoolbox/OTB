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

#ifndef otbPointSetToPointSetFilter_hxx
#define otbPointSetToPointSetFilter_hxx

#include "otbPointSetToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::PointSetToPointSetFilter()
{
  this->SetNumberOfRequiredInputs(1);

}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::SetInput(unsigned int idx, const InputPointSetType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(idx,
                                       const_cast<InputPointSetType *>(input));
  this->GetInput()->ReleaseDataFlagOff();
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::SetInput(const InputPointSetType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(0,
                                       const_cast<InputPointSetType *>(input));
  this->GetInput()->ReleaseDataFlagOff();
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
typename PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>::InputPointSetType *
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::GetInput(unsigned int idx)
{

  return static_cast<InputPointSetType*>
           (this->ProcessObjectType::GetInput(idx));
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
typename PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>::InputPointSetType *
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1) return nullptr;

  return static_cast<InputPointSetType*>
           (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputPointSet, class TOutputPointSet>
void
PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
