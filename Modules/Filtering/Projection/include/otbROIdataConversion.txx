/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbROIdataConversion_txx
#define __otbROIdataConversion_txx

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
