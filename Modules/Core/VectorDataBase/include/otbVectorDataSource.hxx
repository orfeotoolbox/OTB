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


#ifndef otbVectorDataSource_hxx
#define otbVectorDataSource_hxx

#include "otbVectorDataSource.h"

namespace otb
{
/**
 *   Constructor
 */
template <class TOutputVectorData>
VectorDataSource<TOutputVectorData>
::VectorDataSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, OutputVectorDataType::New().GetPointer());
}

template <class TOutputVectorData>
VectorDataSource<TOutputVectorData>
::~VectorDataSource()
{
}

/**
 *
 */

template <class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::GraftOutput(itk::DataObject *graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */

template <class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::GraftNthOutput(unsigned int idx, itk::DataObject *graft)
{
  if ( idx >= this->GetNumberOfOutputs() )
    {
    itkExceptionMacro(<<"Requested to graft output " << idx <<
        " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
    }

  if ( !graft )
    {
    itkExceptionMacro(<<"Requested to graft output that is a NULL pointer" );
    }

  // we use the process object method since all out output may not be
  // of the same type
  itk::DataObject * output = this->ProcessObject::GetOutput(idx);

  // Call GraftImage to copy meta-information, regions, and the pixel container
  output->Graft( graft );
}


template <class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::AllocateOutputs()
{
  OutputVectorDataPointer outputPtr;

  // Allocate the output memory
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
    outputPtr = this->GetOutput(i);
    outputPtr->Clear();
    }
}

/**
 * Get the output vector data
 * \return The vector data produced.
 */
template <class TOutputVectorData>
typename VectorDataSource<TOutputVectorData>::OutputVectorDataType *
VectorDataSource<TOutputVectorData>
::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return nullptr;
    }
  return static_cast<OutputVectorDataType *> (this->ProcessObject::GetOutput(0));
}

template <class TOutputVectorData>
typename VectorDataSource<TOutputVectorData>::OutputVectorDataType *
VectorDataSource<TOutputVectorData>
::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<OutputVectorDataType*>
           (this->Superclass::GetOutput(idx));
}

/**
 * PrintSelf Method
 */
template<class TOutputVectorData>
void
VectorDataSource<TOutputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
