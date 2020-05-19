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

#ifndef otbConcatenateSampleListFilter_hxx
#define otbConcatenateSampleListFilter_hxx

#include "otbConcatenateSampleListFilter.h"
#include "itkProgressReporter.h"

namespace otb
{
namespace Statistics
{

template <class TSampleList>
ConcatenateSampleListFilter<TSampleList>::ConcatenateSampleListFilter()
{
}

template <class TSampleList>
void ConcatenateSampleListFilter<TSampleList>::AddInput(const SampleListType* inputPtr)
{
  // Process object is not const-correct so the const_cast is required here
  Superclass::ProcessObject::AddInput(const_cast<SampleListType*>(inputPtr));
}


template <class TSampleList>
void ConcatenateSampleListFilter<TSampleList>::GenerateData()
{
  // Retrieve output pointers
  SampleListPointer outputSampleListPtr = this->GetOutput();

  // Clear any previous output
  outputSampleListPtr->Clear();

  // Set the measurement vector size (based on the first listsample)
  outputSampleListPtr->SetMeasurementVectorSize(static_cast<SampleListType*>(Superclass::ProcessObject::GetInput(0))->GetMeasurementVectorSize());

  // Evaluate the total number of samples for progress reporting
  unsigned long totalNumberOfSamples = 0;

  for (unsigned int inputIndex = 0; inputIndex < this->GetNumberOfInputs(); ++inputIndex)
  {
    // Retrieve the ListSample
    typename SampleListType::ConstPointer inputPtr = static_cast<SampleListType*>(Superclass::ProcessObject::GetInput(inputIndex));

    totalNumberOfSamples += inputPtr->Size();
  }

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, totalNumberOfSamples);

  for (unsigned int inputIndex = 0; inputIndex < this->GetNumberOfInputs(); ++inputIndex)
  {
    // Retrieve the ListSample
    typename SampleListType::ConstPointer inputSampleListPtr = static_cast<SampleListType*>(Superclass::ProcessObject::GetInput(inputIndex));

    typename SampleListType::ConstIterator inputIt = inputSampleListPtr->Begin();

    // Iterate on the InputSampleList
    while (inputIt != inputSampleListPtr->End())
    {
      // Add the current output sample to the output SampleList
      outputSampleListPtr->PushBack(inputIt.GetMeasurementVector());

      // Update progress
      progress.CompletedPixel();

      ++inputIt;
    }
  }
}

template <class TSampleList>
void ConcatenateSampleListFilter<TSampleList>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of input SampleList: " << this->GetNumberOfInputs();
}


} // End namespace Statistics
} // End namespace otb

#endif
