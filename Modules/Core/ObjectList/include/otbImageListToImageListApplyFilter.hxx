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

#ifndef otbImageListToImageListApplyFilter_hxx
#define otbImageListToImageListApplyFilter_hxx

#include "otbImageListToImageListApplyFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::ImageListToImageListApplyFilter()
{
  m_Filter = FilterType::New();
  m_OutputIndex = 0;
}
/** Generate output information for the ImageList and for each image
      in the List. */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::GenerateOutputInformation()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != inputPtr->Size())
      {
      // in this case, clear the list
      outputPtr->Clear();
      typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
      while (inputListIt != inputPtr->End())
        {
        outputPtr->PushBack(OutputImageType::New());
        ++inputListIt;
        }
      }

    // For each input image
    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
    typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();

    while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
      {
      // Create the output image and set its information

      m_Filter->SetInput(inputListIt.Get());
      m_Filter->UpdateOutputInformation();
      outputListIt.Get()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
      outputListIt.Get()->SetLargestPossibleRegion(m_Filter->GetOutput(m_OutputIndex)->GetLargestPossibleRegion());
      ++inputListIt;
      ++outputListIt;
      }
    }
}
/** Generate input requested region for each image in the List. */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
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
    m_Filter->SetInput(inputListIt.Get());
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_Filter->PropagateRequestedRegion(outputListIt.Get());
    ++inputListIt;
    ++outputListIt;
    }
}
/** Main computation method */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::GenerateData()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
  unsigned int                               counter = 0;

  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    m_Filter->SetInput(inputListIt.Get());
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_Filter->Update();
    outputPtr->SetNthElement(counter, static_cast<OutputImageType *>(m_Filter->GetOutput(m_OutputIndex)));
    outputListIt.Get()->DisconnectPipeline();
    ++inputListIt;
    ++outputListIt;
    ++counter;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
