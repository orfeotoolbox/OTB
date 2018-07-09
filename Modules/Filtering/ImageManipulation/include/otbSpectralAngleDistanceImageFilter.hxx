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

#ifndef otbSpectralAngleDistanceImageFilter_hxx
#define otbSpectralAngleDistanceImageFilter_hxx

#include "otbSpectralAngleDistanceImageFilter.h"
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
SpectralAngleDistanceImageFilter<TInputImage, TOutputImage>
::SpectralAngleDistanceImageFilter()
{
  m_ReferencePixel = 0;
}

template <class TInputImage, class TOutputImage>
void
SpectralAngleDistanceImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  if (this->GetInput()->GetNumberOfComponentsPerPixel() == 1)
    {
    itkExceptionMacro(<< "Not valid input image : mono channel image gives a nul output image.");
    }
}

template <class TInputImage, class TOutputImage>
void
SpectralAngleDistanceImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  if (m_ReferencePixel.Size() == 0)
    {
    itkExceptionMacro(<< "Reference pixel is not set!");
    }

  InputImageConstPointerType inputPtr = this->GetInput();
  OutputImagePointerType     outputPtr = this->GetOutput();

  //inputPtr->UpdateOutputInformation();
  // Check if the reference pixel size matches the input image number of components.
  if (m_ReferencePixel.GetSize() != inputPtr->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Reference pixel size (" << m_ReferencePixel.GetSize() << " and input image pixel size ("
                      << inputPtr->GetNumberOfComponentsPerPixel() << ") don't match!");
    }

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
    double         dist = 0.0;
    double         scalarProd = 0.0;
    double         normProd = 0.0;
    double         normProd1 = 0.0;
    double         normProd2 = 0.0;
    InputPixelType pixel = inputIt.Get();
    for (unsigned int i = 0; i < pixel.Size(); ++i)
      {
      scalarProd += pixel[i] * m_ReferencePixel[i];
      normProd1 += pixel[i] * pixel[i];
      normProd2 += m_ReferencePixel[i] * m_ReferencePixel[i];
      }
    normProd = normProd1 * normProd2;

    if (normProd == 0.0)
      {
      dist = 0.0;
      }
    else
      {
      dist = std::acos(scalarProd / std::sqrt(normProd));
      }
    //------ This part was suppressed since the filter must perform only the spectral angle computation ---
    // Spectral angle normalization
    // dist = dist/(CONST_PI_2);
    //square ponderation
    // dist = std::sqrt(dist);
    outputIt.Set(static_cast<OutputPixelType>(dist));
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
SpectralAngleDistanceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
