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

#ifndef otbUnaryFunctorVectorImageFilter_hxx
#define otbUnaryFunctorVectorImageFilter_hxx

#include "otbUnaryFunctorVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>::UnaryFunctorVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
}

/**
 * Generate the output information
 */
template <class TInputImage, class TOutputImage, class TFunction>
void UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TInputImage, class TOutputImage, class TFunction>
void UnaryFunctorVectorImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                               itk::ThreadIdType threadId)
{
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  itk::ImageRegionConstIterator<InputImageType> inputIt(this->GetInput(), inputRegionForThread);
  inputIt.GoToBegin();

  itk::ImageRegionIterator<OutputImageType> outputIt(this->GetOutput(), outputRegionForThread);
  outputIt.GoToBegin();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while (!outputIt.IsAtEnd() && !inputIt.IsAtEnd())
  {
    outputIt.Set(m_Functor(inputIt.Get()));

    ++inputIt;
    ++outputIt;

    progress.CompletedPixel();
  }
}

} // end namespace otb

#endif
