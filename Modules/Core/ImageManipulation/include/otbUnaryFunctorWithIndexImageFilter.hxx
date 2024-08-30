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

#ifndef otbUnaryFunctorWithIndexImageFilter_hxx
#define otbUnaryFunctorWithIndexImageFilter_hxx

#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryFunctorWithIndexImageFilter<TInputImage, TOutputImage, TFunction>::UnaryFunctorWithIndexImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
template <class TInputImage, class TOutputImage, class TFunction>
void UnaryFunctorWithIndexImageFilter<TInputImage, TOutputImage, TFunction>::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr  = const_cast<TInputImage*>(this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
  {
    return;
  }
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
  {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream               msg;
    msg << this->GetNameOfClass() << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage, class TFunction>
void UnaryFunctorWithIndexImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                                  itk::ThreadIdType threadId)
{
  InputImagePointer  inputPtr  = dynamic_cast<const TInputImage*>(ProcessObjectType::GetInput(0));
  OutputImagePointer outputPtr = this->GetOutput(0);

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  IteratorType                           inputIt = IteratorType(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd())
  {
    outputIt.Set(m_Functor(inputIt.Get(), inputIt.GetIndex()));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
  }
}

} // end namespace otb

#endif
