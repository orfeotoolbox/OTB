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


#ifndef otbImageListToSingleImageFilter_hxx
#define otbImageListToSingleImageFilter_hxx

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "otbImageListToSingleImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TImageType>
ImageListToSingleImageFilter<TImageType>
::ImageListToSingleImageFilter()
 :m_ExtractedImagePosition(0)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}

template <class TImageType>
typename ImageListToSingleImageFilter<TImageType>::OutputImagePointerType
ImageListToSingleImageFilter<TImageType>
::GetOutput()
{
  // If there is no input
  if (this->GetNumberOfInputs() != 1)
    {
    // exit
    return nullptr;
    }
  // else return the first input
  return static_cast<OutputImagePointerType>(
       this->GetInput()->GetNthElement(m_ExtractedImagePosition));
}

/**
 * Main computation method
 */
template <class TImageType>
void
ImageListToSingleImageFilter<TImageType>
::GenerateData(void)
{
  OutputImagePointerType  outputPtr = this->GetOutput();
  InputImagePointerType inputPtr = this->GetInput()->GetNthElement(m_ExtractedImagePosition);

  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType>     OutputIteratorType;

  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  itk::ProgressReporter progress(this, 0, outputPtr->GetRequestedRegion().GetNumberOfPixels());

  OutputIteratorType outputIt(outputPtr, outputPtr->GetRequestedRegion());
  outputIt.GoToBegin();

  InputIteratorType inputIt(inputPtr, outputPtr->GetRequestedRegion());
  inputIt.GoToBegin();
  while (!inputIt.IsAtEnd())
    {

    if ( !(outputIt).IsAtEnd())
      {
      outputIt.Set(inputIt.Get());
      ++outputIt;
      }
    else
      {
      itkGenericExceptionMacro("End of image at index " << outputIt.GetIndex() << " !");
      }
    ++inputIt;
    }
}

/**
 * PrintSelf Method
 */
template <class TImageType>
void
ImageListToSingleImageFilter<TImageType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
