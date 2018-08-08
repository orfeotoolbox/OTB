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

#ifndef otbVectorImageToIntensityImageFilter_hxx
#define otbVectorImageToIntensityImageFilter_hxx

#include "otbVectorImageToIntensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::VectorImageToIntensityImageFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
}

template <class TInputImage, class TOutputImage>
void
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  InputImageConstPointerType inputPtr = this->GetInput();
  OutputImagePointerType     outputPtr = this->GetOutput();

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
    {
    double         sum = 0.0;
    InputPixelType pixel = inputIt.Get();
    for (unsigned int i = 0; i < pixel.Size(); ++i)
      {
      sum += pixel[i];
      }
    sum /= pixel.Size();

    outputIt.Set(static_cast<OutputPixelType>(sum));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
