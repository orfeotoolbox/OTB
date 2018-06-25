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

#ifndef otbUnaryImageFunctorWithVectorImageFilter_hxx
#define otbUnaryImageFunctorWithVectorImageFilter_hxx

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryImageFunctorWithVectorImageFilter<TInputImage, TOutputImage, TFunction>
::UnaryImageFunctorWithVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->InPlaceOff();
}

/**
 * UnaryImageFunctorWithVectorImageFilter can produce an image which is a different resolution
 * than its input image. As such, UnaryImageFunctorWithVectorImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model. The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformation()
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryImageFunctorWithVectorImageFilter<TInputImage, TOutputImage, TFunction>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();

  if (!outputPtr || !inputPtr)
    {
    return;
    }
  outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
    inputPtr->GetNumberOfComponentsPerPixel());

  // TODO: Check this
  // The Functor vector is not initialised !
  for (unsigned int i = 0; i < inputPtr->GetNumberOfComponentsPerPixel(); ++i)
    {
    FunctorType functor;
    m_FunctorVector.push_back(functor);
    }
}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryImageFunctorWithVectorImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer inputPtr  = this->GetInput();

  // Define the iterators
  itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<OutputImageType>      outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  // Null pixel construction
  InputPixelType nullPixel;
  nullPixel.SetSize(inputPtr->GetNumberOfComponentsPerPixel());
  nullPixel.Fill(itk::NumericTraits<OutputInternalPixelType>::Zero);

  while (!inputIt.IsAtEnd())
    {
    InputPixelType  inPixel = inputIt.Get();
    OutputPixelType outPixel;
    outPixel.SetSize(inputPtr->GetNumberOfComponentsPerPixel());
    outPixel.Fill(itk::NumericTraits<OutputInternalPixelType>::Zero);
    // if the input pixel in null, the output is considered as null ( no sensor information )
    if (inPixel != nullPixel)
      {
      for (unsigned int j = 0; j < inputPtr->GetNumberOfComponentsPerPixel(); ++j)
        {
        outPixel[j] = m_FunctorVector[j](inPixel[j]);
        }
      }
    outputIt.Set(outPixel);
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}

template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryImageFunctorWithVectorImageFilter<TInputImage, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
