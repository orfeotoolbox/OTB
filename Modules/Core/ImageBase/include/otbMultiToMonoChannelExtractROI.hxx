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

#ifndef otbMultiToMonoChannelExtractROI_hxx
#define otbMultiToMonoChannelExtractROI_hxx

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{

/**
 *
 */
template <class TInputPixelType, class TOutputPixelType>
MultiToMonoChannelExtractROI<TInputPixelType, TOutputPixelType>::MultiToMonoChannelExtractROI()
  : ExtractROIBase<VectorImage<TInputPixelType, 2>, Image<TOutputPixelType, 2>>(), m_Channel(1)
{
}

/**
 *
 */
template <class TInputPixelType, class TOutputPixelType>
void MultiToMonoChannelExtractROI<TInputPixelType, TOutputPixelType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

/**
 * ExtractImageFilter can produce an image which is a different resolution
 * than its input image.  As such, ExtractImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton()
 */
template <class TInputPixelType, class TOutputPixelType>
void MultiToMonoChannelExtractROI<TInputPixelType, TOutputPixelType>::GenerateOutputInformation()
{
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  // Bounds checking for the channel to process
  if ((m_Channel <= 0) || (m_Channel > inputPtr->GetVectorLength()))
  {
    itkExceptionMacro(<< "otb::MultiToMonoChannelExtractROI::GenerateOutputInformation "
                      << "The selected channel must in the range [1;" << inputPtr->GetVectorLength() << "] "
                      << typeid(itk::ImageBase<InputImageDimension>*).name());
  }

  // Calling the superclass method
  Superclass::GenerateOutputInformation();
}

template <class TInputPixelType, class TOutputPixelType>
void MultiToMonoChannelExtractROI<TInputPixelType, TOutputPixelType>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                           itk::ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");

  // Get the input and output pointers
  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();
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
  InputIterator  inIt(inputPtr, inputRegionForThread);

  // Go through channels to process
  const unsigned int channelIn(m_Channel - 1);

  while (!outIt.IsAtEnd())
  {

    InputImagePixelType const& pixelInput  = inIt.Get();
    OutputImagePixelType const pixelOutput = static_cast<OutputValueType>(pixelInput[channelIn]);
    outIt.Set(pixelOutput);
    ++outIt;
    ++inIt;
    progress.CompletedPixel();
  }
}

} // end namespace otb

#endif
