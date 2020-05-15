/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbFunctionWithNeighborhoodToImageFilter_hxx
#define otbFunctionWithNeighborhoodToImageFilter_hxx

#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbMacro.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
FunctionWithNeighborhoodToImageFilter<TInputImage, TOutputImage, TFunction>::FunctionWithNeighborhoodToImageFilter()
{
  this->InPlaceOff();
  this->SetNumberOfRequiredInputs(1);
  m_Radius.Fill(1);
  m_Offset.Fill(1);
  m_FunctionList.clear();
  m_Function = FunctionType::New();
}

template <class TInputImage, class TOutputImage, class TFunction>
void FunctionWithNeighborhoodToImageFilter<TInputImage, TOutputImage, TFunction>::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  InputImageConstPointer inputPtr = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));
  if (inputPtr.IsNull())
  {
    itkExceptionMacro(<< "At least one input is missing."
                      << " Input is missing :" << inputPtr.GetPointer();)
  }
  m_Function->SetInputImage(inputPtr);
  for (unsigned int i = 0; i < static_cast<unsigned int>(this->GetNumberOfThreads()); ++i)
  {
    FunctionPointerType func = m_Function;
    m_FunctionList.push_back(func);
  }
}

template <class TInputImage, class TOutputImage, class TFunction>
void FunctionWithNeighborhoodToImageFilter<TInputImage, TOutputImage, TFunction>::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointer  inputPtr  = const_cast<TInputImage*>(this->GetInput());
  OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
  {
    return;
  }
  // get a copy of the input requested region (should equal the output
  // requested region)
  InputImageRegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  InputImageSizeType maxRad;
  maxRad[0] = m_Radius[0] + std::abs(m_Offset[0]);
  maxRad[1] = m_Radius[0] + std::abs(m_Offset[1]);
  inputRequestedRegion.PadByRadius(maxRad);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
  {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream               msg;
    msg << this->GetNameOfClass() << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction>
void FunctionWithNeighborhoodToImageFilter<TInputImage, TOutputImage, TFunction>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                                       itk::ThreadIdType threadId)
{
  // We use dynamic_cast since inputs are stored as DataObjects.
  InputImageConstPointer inputPtr = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));

  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<TOutputImage>     outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd())
  {
    outputIt.Set(static_cast<OutputImagePixelType>(m_FunctionList[threadId]->EvaluateAtIndex(inputIt.GetIndex())));
    ++inputIt;
    ++outputIt;

    progress.CompletedPixel(); // potential exception thrown here
  }
}
} // end namespace otb

#endif
