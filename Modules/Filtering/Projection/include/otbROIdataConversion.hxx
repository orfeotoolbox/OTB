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


#ifndef otbROIdataConversion_hxx
#define otbROIdataConversion_hxx

#include "itkVector.h"

#include "otbROIdataConversion.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TInputImage, class TInputROIImage>
ROIdataConversion<TInputImage, TInputROIImage>
::ROIdataConversion()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
}

template <class TInputImage, class TInputROIImage>
void
ROIdataConversion<TInputImage, TInputROIImage>
::GenerateOutputInformation(void)
{
  typename OutputImageType::Pointer  outputPtr = this->GetOutput();
  typename OutputImageType::SizeType outputSize = outputPtr->GetRequestedRegion().GetSize();
  outputSize[0] = GetNumberOfSample();
  outputPtr->SetRegions(outputSize);
}

template <class TInputImage, class TInputROIImage>
void
ROIdataConversion<TInputImage, TInputROIImage>
::GenerateInputRequestedRegion(void)
{
  typename InputImageType::Pointer    inputImagePtr = const_cast<InputImageType *>(this->GetInputImage());
  typename InputROIImageType::Pointer inputROIPtr = this->GetROIImage();
  inputImagePtr->SetRequestedRegion(inputImagePtr->GetLargestPossibleRegion());
  inputROIPtr->SetRequestedRegion(inputROIPtr->GetLargestPossibleRegion());
}

template <class TInputImage, class TInputROIImage>
void
ROIdataConversion<TInputImage, TInputROIImage>
::GenerateData()
{
  typename InputImageType::Pointer         inputImagePtr = const_cast<InputImageType *>(this->GetInputImage());
  typename InputROIImageType::ConstPointer inputROIPtr = this->GetROIImage();
  typename OutputImageType::Pointer        outputPtr = this->GetOutput();

  outputPtr->Allocate();
  typename InputImageType::PixelType zero;
  itk::NumericTraits<typename InputImageType::PixelType>::SetLength(zero, outputPtr->GetNumberOfComponentsPerPixel());
  outputPtr->FillBuffer(zero);

  itk::ImageRegionConstIterator<InputImageType> inputIter
    (inputImagePtr, inputImagePtr->GetRequestedRegion());
  itk::ImageRegionConstIterator<InputROIImageType> trainingIter
    (inputROIPtr, inputROIPtr->GetRequestedRegion());
  itk::ImageRegionIterator<OutputImageType> outputIter
    (outputPtr, outputPtr->GetRequestedRegion());

  inputIter.GoToBegin();
  trainingIter.GoToBegin();
  outputIter.GoToBegin();

  while (!trainingIter.IsAtEnd())
    {
    if (trainingIter.Get() != 0)
      {
      outputIter.Set(inputIter.Get());
      ++outputIter;
      }

    ++inputIter;
    ++trainingIter;
    }
}

template <class TInputImage, class TInputROIImage>
typename ROIdataConversion<TInputImage, TInputROIImage>::SizeValueType
ROIdataConversion<TInputImage, TInputROIImage>
::GetNumberOfSample()
{
  InputROIImagePointerType inputROIPtr = GetROIImage();
  itk::ImageRegionConstIterator<InputROIImageType> trainingIter(inputROIPtr, inputROIPtr->GetRequestedRegion());

  trainingIter.GoToBegin();

  SizeValueType count = 0L;
  while (!trainingIter.IsAtEnd())
    {
    if (trainingIter.Get() != 0) count++;
    ++trainingIter;
    }

  return count;
}

} // end of namespace otb

#endif
