/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorDataToSpecificDescriptionFilterBase_hxx
#define otbVectorDataToSpecificDescriptionFilterBase_hxx

#include "otbVectorDataToSpecificDescriptionFilterBase.h"

namespace otb
{

// Constructor
template <class TVectorData>
VectorDataToSpecificDescriptionFilterBase<TVectorData>::VectorDataToSpecificDescriptionFilterBase()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TVectorData>
void VectorDataToSpecificDescriptionFilterBase<TVectorData>::AddSupport(const DataObject* support, unsigned int idx)
{
  // Process object is not const-correct so the const_cast is required here
  // input(0) is dedicated to the input vector data, support #i is
  // stored in input(i+1)
  this->itk::ProcessObject::SetNthInput(idx + 1, const_cast<DataObject*>(support));
}

template <class TVectorData>
const typename VectorDataToSpecificDescriptionFilterBase<TVectorData>::DataObject*
VectorDataToSpecificDescriptionFilterBase<TVectorData>::GetSupport(unsigned int idx)
{
  if (this->GetNumberOfInputs() < idx + 2)
  {
    return nullptr;
  }

  return static_cast<const DataObject*>(this->itk::ProcessObject::GetInput(idx + 1));
}

// PrintSelf Method
template <class TVectorData>
void VectorDataToSpecificDescriptionFilterBase<TVectorData>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of Support: " << (int)(this->GetNumberOfInputs()) - 1 << std::endl;
}

} // end namespace otb

#endif
