/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2016-2019 IRSTEA
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
#ifndef __SummingFilter_hxx
#define __SummingFilter_hxx

#include "otbSummingFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Generates output information:
 */
template <class TInputImage, class TOutputImage>
void SummingFilter<TInputImage, TOutputImage>::GenerateOutputInformation(void)
{
  itkDebugMacro(<< "Generate output informations");
  Superclass::GenerateOutputInformation();
}

/**
 * Processing.
 */
template <class TInputImage, class TOutputImage>
void SummingFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  // Debug info
  itkDebugMacro(<< "Actually executing thread " << threadId << " in region " << outputRegionForThread);

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Iterate through the thread region
  OutputIteratorType outputIt(this->GetOutput(), outputRegionForThread);

  // Prepare input iterators
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int nbInputBands;

  InputIteratorType inputIt[nbInputImages];
  for (unsigned int i = 0; i < nbInputImages; i++)
  {
    InputImageType* currentImage = const_cast<InputImageType*>(this->GetInput(i));
    inputIt[i]                   = InputIteratorType(currentImage, outputRegionForThread);
    nbInputBands                 = currentImage->GetNumberOfComponentsPerPixel();
  }

  OutputImagePixelType pix;
  pix.SetSize(nbInputBands);

  // Sum images
  for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
  {
    pix.Fill(0.0);
    for (unsigned int i = 0; i < nbInputImages; i++)
    {

      for (unsigned int band = 0; band < nbInputBands; band++)
      {
        pix[band] += inputIt[i].Get()[band];
      }
      ++inputIt[i];
    }
    outputIt.Set(pix);

    // Update process
    progress.CompletedPixel();
  }
}
}

#endif
