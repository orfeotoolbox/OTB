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

#ifndef otbVectorImageTo3DScalarImageFilter_hxx
#define otbVectorImageTo3DScalarImageFilter_hxx

#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;
#include "otbVectorImageTo3DScalarImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageSliceIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::VectorImageTo3DScalarImageFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
}
/** Generate output information */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{

  const InputImageType* inputPtr = this->GetInput();
  OutputImageType*      outputPtr = this->GetOutput();

  InputImageRegionType inputLargestRegion = this->GetInput()->GetLargestPossibleRegion();
  OutputImageSizeType  size;
  OutputImageIndexType index;
  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = inputLargestRegion.GetSize()[i];
    index[i] = inputLargestRegion.GetIndex()[i];
    }
  size[OutputImageType::ImageDimension - 1] = inputPtr->GetNumberOfComponentsPerPixel();
  index[OutputImageType::ImageDimension - 1] = 0;

  OutputImageRegionType outputRegion;
  outputRegion.SetSize(size);
  outputRegion.SetIndex(index);
  outputPtr->SetLargestPossibleRegion(outputRegion);
}
/** Generate input requested region */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  InputImageType*  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImageType* outputPtr = this->GetOutput();

  OutputImageRegionType requestedRegion = outputPtr->GetRequestedRegion();
  InputImageRegionType  inputRequestedRegion;
  InputImageSizeType    size;
  InputImageIndexType   index;

  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = requestedRegion.GetSize()[i];
    index[i] = requestedRegion.GetIndex()[i];
    }
  inputRequestedRegion.SetSize(size);
  inputRequestedRegion.SetIndex(index);
  inputPtr->SetRequestedRegion(inputRequestedRegion);
}
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType itkNotUsed(threadId))
{
  const InputImageType* inputPtr = this->GetInput();
  OutputImageType*      outputPtr = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType>     InputIteratorType;
  typedef itk::ImageSliceIteratorWithIndex<OutputImageType> OutputIteratorType;

  InputImageRegionType inputRegion;
  InputImageSizeType   size;
  InputImageIndexType  index;

  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = outputRegionForThread.GetSize()[i];
    index[i] = outputRegionForThread.GetIndex()[i];
    }
  inputRegion.SetSize(size);
  inputRegion.SetIndex(index);

  InputIteratorType inIt(inputPtr, inputRegion);

  OutputIteratorType outIt(outputPtr, outputRegionForThread);
  outIt.SetFirstDirection(0);
  outIt.SetSecondDirection(1);

  outIt.GoToBegin();
  inIt.GoToBegin();

  while (!outIt.IsAtEnd())
    {
    outIt.Set(static_cast<OutputPixelType>(inIt.Get()[outIt.GetIndex()[InputImageType::ImageDimension]]));
    ++inIt;
    if (inIt.IsAtEnd())
      {
      inIt.GoToBegin();
      }
    ++outIt;
    if (outIt.IsAtEndOfLine())
      {
      outIt.NextLine();
      }
    if (outIt.IsAtEndOfSlice())
      {
      outIt.NextSlice();
      }
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
