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

#ifndef otbExtractROI_hxx
#define otbExtractROI_hxx

#include "otbExtractROI.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputPixel, class TOutputPixel>
ExtractROI<TInputPixel, TOutputPixel>
::ExtractROI() //: ExtractROIBase< itk::Image<TInputPixel, VImageDimension> , itk::Image<TOutputPixel, VImageDimension> >()
{
  OTB_DISABLE_DYNAMIC_MT
}

/**
 *
 */
template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

/**
 * ExtractROI can produce an image which is a different resolution
 * than its input image.  As such, ExtractROI needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton()
 */
template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::GenerateOutputInformation()
{
  // Call to the base class method
  Superclass::GenerateOutputInformation();
}

template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");

  // Get the input and output pointers
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define the portion of the input to walk for this thread
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators.
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIterator;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  // walk the output region, and sample the input image
  while (!outIt.IsAtEnd())
    {
    // copy the input pixel to the output
    outIt.Set(inIt.Get());
    ++outIt;
    ++inIt;
    progress.CompletedPixel();
    }
}

} // end namespace otb

#endif
