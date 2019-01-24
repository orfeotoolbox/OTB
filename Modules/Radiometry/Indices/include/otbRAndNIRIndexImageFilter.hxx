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

#ifndef otbRAndNIRIndexImageFilter_hxx
#define otbRAndNIRIndexImageFilter_hxx

#include "otbRAndNIRIndexImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageR, class TInputImageNIR,
    class TOutputImage, class TFunction>
RAndNIRIndexImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction>
::RAndNIRIndexImageFilter()
{
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndNIRIndexImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction>
::SetInputR(const TInputImageR * image1)
{
  // Process object is not const-correct so the const casting is required.
  this->SetInput1(image1);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndNIRIndexImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction>
::SetInputNIR(const TInputImageNIR * image2)
{
  // Process object is not const-correct so the const casting is required.
//  SetNthInput(1, const_cast<TInputImageNIR *>( image2 ));
  this->SetInput2(image2);
}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
/*
template <class TInputImageR, class TInputImageNIR, class TOutputImage, class TFunction  >
void
RAndNIRIndexImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        itk::ThreadIdType threadId)
{

  // We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImageR so it cannot be used for the second input.
  RInputImagePointer inputPtr1
    = dynamic_cast<const TInputImageR*>(itk::ProcessObject::GetInput(0));
  NIRInputImagePointer inputPtr2
    = dynamic_cast<const TInputImageNIR*>(itk::ProcessObject::GetInput(1));
  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImageR> inputItR(inputPtr1, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImageNIR> inputItNIR(inputPtr2, outputRegionForThread);

  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputItR.GoToBegin();
  inputItNIR.GoToBegin();
  outputIt.GoToBegin();

  while( !inputItR.IsAtEnd() )
    {
    outputIt.Set( this->GetFunctor()( inputItR.Get(), inputItNIR.Get() ) );
    ++inputItNIR;
    ++inputItR;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
    }

}
*/
template <class TInputImageR, class TInputImageNIR, class TOutputImage, class TFunction>
void
RAndNIRIndexImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
