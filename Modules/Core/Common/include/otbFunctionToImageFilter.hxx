/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbFunctionToImageFilter_hxx
#define otbFunctionToImageFilter_hxx

#include "otbFunctionToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
FunctionToImageFilter<TInputImage, TOutputImage, TFunction>::FunctionToImageFilter()
{
  this->InPlaceOff();
  m_PixelFunction = FunctionType::New();
}

/**
 * Connect one of the operands for pixel-wise addition
 */
// template<class TInputImage, class TOutputImage, class TFunction >
// void
// FunctionToImageFilter<TInputImage, TOutputImage, TFunction>
// ::SetInput( const TInputImage *image )
// {
//   // The ProcessObject is not const-correct so the const_cast is required here
//   SetNthInput( 0, const_cast<TInputImage *>( image ) );
// }

/**
 * BeforeThreadedGenerateData function. Validate inputs
 */
template <class TInputImage, class TOutputImage, class TFunction>
void FunctionToImageFilter<TInputImage, TOutputImage, TFunction>::BeforeThreadedGenerateData()
{
  InputImagePointer inputPtr = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));
  if (inputPtr.IsNull())
  {
    itkExceptionMacro(<< "At least one input is missing."
                      << " Input is missing :" << inputPtr.GetPointer();)
  }
  m_PixelFunction->SetInputImage(inputPtr);
}

/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction>
void FunctionToImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                       itk::ThreadIdType threadId)
{

  // We use dynamic_cast since inputs are stored as DataObjects.
  InputImagePointer inputPtr = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));

  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<TOutputImage>     outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();


  while (!inputIt.IsAtEnd())
  {
    outputIt.Set(static_cast<OutputImagePixelType>(m_PixelFunction->EvaluateAtIndex(inputIt.GetIndex())));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
  }
}
} // end namespace otb

#endif
