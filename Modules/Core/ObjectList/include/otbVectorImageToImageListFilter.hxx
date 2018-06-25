/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorImageToImageListFilter_hxx
#define otbVectorImageToImageListFilter_hxx

#include "otbVectorImageToImageListFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include <vector>
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{
/** Generate the input requested region from the first element in the list. */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType, TImageList>
::GenerateOutputInformation(void)
{
  OutputImageListPointerType  outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();

  if (inputPtr)
    {
    if (outputPtr->Size() != inputPtr->GetNumberOfComponentsPerPixel())
      {
      // if the number of components does not match, clear the list
      outputPtr->Clear();
      for (unsigned int i = 0; i < inputPtr->GetNumberOfComponentsPerPixel(); ++i)
        {
        typename OutputImageType::Pointer tmpImagePtr = OutputImageType::New();
        this->AddOutput(tmpImagePtr);
        outputPtr->PushBack(tmpImagePtr);
        }
      }
    for (unsigned int i = 0; i < inputPtr->GetNumberOfComponentsPerPixel(); ++i)
      {
      typename OutputImageType::Pointer tmpImagePtr = outputPtr->GetNthElement(i);
      tmpImagePtr->CopyInformation(inputPtr);
      tmpImagePtr->SetLargestPossibleRegion(inputPtr->GetLargestPossibleRegion());
      tmpImagePtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());
      }
    }
}
/** Generate the output information by building the output list. */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType, TImageList>
::GenerateInputRequestedRegion(void)
{
  OutputImageListPointerType  outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();

  if (inputPtr)
    {
    if (outputPtr->Size() > 0)
      {
      inputPtr->SetRequestedRegion(outputPtr->GetNthElement(0)->GetRequestedRegion());
      }
    }
}
/**
 * Main computation method
 */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType, TImageList>
::GenerateData(void)
{
  OutputImageListPointerType  outputPtr = this->GetOutput();
  InputVectorImagePointerType inputPtr = this->GetInput();

  typedef itk::ImageRegionConstIteratorWithIndex<InputVectorImageType> InputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType>           OutputIteratorType;

  std::vector<OutputIteratorType> outputIteratorList;

  typename OutputImageListType::ConstIterator outputListIt = outputPtr->Begin();
  for (; outputListIt != outputPtr->End(); ++outputListIt)
    {
    outputListIt.Get()->SetBufferedRegion(outputListIt.Get()->GetRequestedRegion());
    outputListIt.Get()->Allocate();
    OutputIteratorType tmpIt = OutputIteratorType(outputListIt.Get(), outputListIt.Get()->GetRequestedRegion());
    tmpIt.GoToBegin();
    outputIteratorList.push_back(tmpIt);
    }

  InputIteratorType inputIt(inputPtr, outputPtr->GetNthElement(0)->GetRequestedRegion());

  itk::ProgressReporter progress(this, 0, outputPtr->GetNthElement(0)->GetRequestedRegion().GetNumberOfPixels());

  inputIt.GoToBegin();
  while (!inputIt.IsAtEnd())
    {
    unsigned int counter = 0;
    for (typename std::vector<OutputIteratorType>::iterator it = outputIteratorList.begin();
         it != outputIteratorList.end(); ++it)
      {
      if (!(*it).IsAtEnd())
        {
        (*it).Set(static_cast<typename OutputImageType::PixelType>(inputIt.Get()[counter]));
        ++(*it);
        ++counter;
        }
      else
        {
        itkGenericExceptionMacro("End of image for band " << counter << " at index " << (*it).GetIndex() << " !");
        }
      }
    progress.CompletedPixel();
    ++inputIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TVectorImageType, class TImageList>
void
VectorImageToImageListFilter<TVectorImageType, TImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
