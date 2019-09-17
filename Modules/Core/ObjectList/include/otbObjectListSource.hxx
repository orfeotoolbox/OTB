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

#ifndef otbObjectListSource_hxx
#define otbObjectListSource_hxx

#include "otbObjectListSource.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
   * Constructor
 */
template <class TOutputList>
ObjectListSource<TOutputList>::ObjectListSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputImage
  typename TOutputList::Pointer output = static_cast<TOutputList*>(this->MakeOutput(0).GetPointer());
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, output.GetPointer());
}

/**
 *
 */
template <class TOutputList>
typename ObjectListSource<TOutputList>::DataObjectPointer ObjectListSource<TOutputList>::MakeOutput(DataObjectPointerArraySizeType)
{
  return static_cast<itk::DataObject*>(TOutputList::New().GetPointer());
}

/**
 *
 */
template <class TOutputList>
typename ObjectListSource<TOutputList>::OutputListType* ObjectListSource<TOutputList>::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }

  return static_cast<TOutputList*>(this->Superclass::GetOutput(0));
}

/**
 *
 */
template <class TOutputList>
typename ObjectListSource<TOutputList>::OutputListType* ObjectListSource<TOutputList>::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<TOutputList*>(this->Superclass::GetOutput(idx));
}

/**
 *
 */
template <class TOutputList>
void ObjectListSource<TOutputList>::GraftOutput(itk::DataObject* graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template <class TOutputList>
void ObjectListSource<TOutputList>::GraftNthOutput(DataObjectPointerArraySizeType idx, itk::DataObject* graft)
{
  if (idx >= this->GetNumberOfOutputs())
  {
    itkExceptionMacro(<< "Requested to graft output " << idx << " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
  }

  if (!graft)
  {
    itkExceptionMacro(<< "Requested to graft output that is a NULL pointer");
  }

  itk::DataObject* output = this->GetOutput(idx);

  // Call GraftImage to copy meta-information, regions, and the pixel container
  output->Graft(graft);
}

//----------------------------------------------------------------------------

template <class TOutputList>
void ObjectListSource<TOutputList>::AllocateOutputs()
{
  OutputListPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    outputPtr = this->GetOutput(i);
    outputPtr->Clear();
  }
}

/**
   * GenerateData
 */
template <class TOutputList>
void ObjectListSource<TOutputList>::GenerateData(void)
{
  itkExceptionMacro("subclass should override this method!!!");
}

/**
   * PrintSelf Method
 */
template <class TOutputList>
void ObjectListSource<TOutputList>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
