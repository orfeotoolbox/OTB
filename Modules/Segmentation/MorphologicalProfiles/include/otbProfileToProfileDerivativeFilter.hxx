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

#ifndef otbProfileToProfileDerivativeFilter_hxx
#define otbProfileToProfileDerivativeFilter_hxx

#include "otbProfileToProfileDerivativeFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImageList, class TOutputImageList>
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::ProfileToProfileDerivativeFilter()
{
  m_SubtractFilter = SubtractFilterType::New();
  m_AbsFilter = AbsFilterType::New();
}
/** Generate output information for the ImageList and for each image
      in the List. */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateOutputInformation()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != inputPtr->Size() - 1)
      {
      // in this case, clear the list
      outputPtr->Clear();
      typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
      ++inputListIt;
      while (inputListIt != inputPtr->End())
        {
        outputPtr->PushBack(OutputImageType::New());
        ++inputListIt;
        }
      }

    // For each input image
    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
    typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
    ++inputListIt;
    while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
      {
      // Create the output image and set its information
      outputListIt.Get()->CopyInformation(inputListIt.Get());
      outputListIt.Get()->SetLargestPossibleRegion(inputListIt.Get()->GetLargestPossibleRegion());
      ++inputListIt;
      ++outputListIt;
      }
    }
}
/** Generate input requested region for each image in the List. */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateInputRequestedRegion()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();

  // Use the filter to generate input requested region
  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    inputListIt.Get()->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    ++inputListIt;
    ++outputListIt;
    }
}
/** Main computation method */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateData()
{
  m_AbsFilter->SetInput(m_SubtractFilter->GetOutput());

  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
  unsigned int                               counter = 0;

  InputImagePointerType lastImage = inputListIt.Get();
  ++inputListIt;

  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    m_SubtractFilter->SetInput1(inputListIt.Get());
    m_SubtractFilter->SetInput2(lastImage);
    m_AbsFilter->GetOutput()->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_AbsFilter->Update();
    outputPtr->SetNthElement(counter, static_cast<OutputImageType *>(m_AbsFilter->GetOutput()));
    outputListIt.Get()->DisconnectPipeline();
    lastImage = inputListIt.Get();
    ++inputListIt;
    ++outputListIt;
    ++counter;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
