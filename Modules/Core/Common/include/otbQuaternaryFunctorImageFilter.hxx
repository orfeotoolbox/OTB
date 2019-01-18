/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbQuaternaryFunctorImageFilter_hxx
#define otbQuaternaryFunctorImageFilter_hxx

#include "otbQuaternaryFunctorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::QuaternaryFunctorImageFilter()
{
  this->InPlaceOff();
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::SetInput1(const TInputImage1 *image1)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(0, const_cast<TInputImage1 *>(image1));
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::SetInput2(const TInputImage2 *image2)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(1, const_cast<TInputImage2 *>(image2));
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::SetInput3(const TInputImage3 *image3)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(2, const_cast<TInputImage3 *>(image3));
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::SetInput4(const TInputImage4 *image4)
{
  // The ProcessObject is not const-correct so the const_cast is required here
  this->SetNthInput(3, const_cast<TInputImage4 *>(image4));
}

/**
 * BeforeThreadedGenerateData function. Validate inputs
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::BeforeThreadedGenerateData()
{
  Input1ImagePointer inputPtr1
    = dynamic_cast<const TInputImage1*>((itk::ProcessObject::GetInput(0)));
  Input2ImagePointer inputPtr2
    = dynamic_cast<const TInputImage2*>((itk::ProcessObject::GetInput(1)));
  Input3ImagePointer inputPtr3
    = dynamic_cast<const TInputImage3*>((itk::ProcessObject::GetInput(2)));
  Input4ImagePointer inputPtr4
    = dynamic_cast<const TInputImage4*>((itk::ProcessObject::GetInput(3)));
  if (inputPtr1.IsNull() || inputPtr2.IsNull() || inputPtr3.IsNull() || inputPtr4.IsNull())
    {
    itkExceptionMacro(<< "At least one input is missing."
                      << " Input1 is " << inputPtr1.GetPointer() << ", "
                      << " Input2 is " << inputPtr2.GetPointer() << ", "
                      << " Input3 is " << inputPtr3.GetPointer() << ", "
                      << " Input4 is " << inputPtr4.GetPointer());

    }
}

/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template <class TInputImage1, class TInputImage2, class TInputImage3,
    class TInputImage4, class TOutputImage, class TFunction>
void
QuaternaryFunctorImageFilter<TInputImage1, TInputImage2, TInputImage3, TInputImage4, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  // We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImage1 so it cannot be used for the second or third input.
  Input1ImagePointer inputPtr1
    = dynamic_cast<const TInputImage1*>((itk::ProcessObject::GetInput(0)));
  Input2ImagePointer inputPtr2
    = dynamic_cast<const TInputImage2*>((itk::ProcessObject::GetInput(1)));
  Input3ImagePointer inputPtr3
    = dynamic_cast<const TInputImage3*>((itk::ProcessObject::GetInput(2)));
  Input4ImagePointer inputPtr4
    = dynamic_cast<const TInputImage4*>((itk::ProcessObject::GetInput(3)));
  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImage1> inputIt1(inputPtr1, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImage2> inputIt2(inputPtr2, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImage3> inputIt3(inputPtr3, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImage4> inputIt4(inputPtr4, outputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt1.GoToBegin();
  inputIt2.GoToBegin();
  inputIt3.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt1.IsAtEnd())
    {
    outputIt.Set(m_Functor(inputIt1.Get(), inputIt2.Get(), inputIt3.Get(), inputIt4.Get()));
    ++inputIt1;
    ++inputIt2;
    ++inputIt3;
    ++inputIt4;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
    }
}
} // end namespace otb

#endif
