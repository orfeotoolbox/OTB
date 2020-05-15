/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbDecimateImageFilter_hxx
#define otbDecimateImageFilter_hxx

#include "otbDecimateImageFilter.h"

#include "otbMacro.h"
#include "otbSubsampledImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
void DecimateImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "DecimationFactor = " << m_DecimationFactor << "\n";
}

template <class TInputImage, class TOutputImage>
void DecimateImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if (GetDecimationFactor() > 1)
  {
    this->GetOutput()->CopyInformation(this->GetInput());

    OutputImageRegionType newRegion;
    this->CallCopyInputRegionToOutputRegion(newRegion, this->GetInput()->GetLargestPossibleRegion());
    this->GetOutput()->SetRegions(newRegion);

    for (unsigned int i = 0; i < OutputImageDimension; ++i)
    {
      otbGenericMsgDebugMacro(<< "Image Output size [" << i << "] = " << newRegion.GetSize(i));
    }
  }
}

template <class TInputImage, class TOutputImage>
void DecimateImageFilter<TInputImage, TOutputImage>::CallCopyOutputRegionToInputRegion(InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
  typename OutputImageRegionType::SizeType  srcSize  = srcRegion.GetSize();

  typename InputImageRegionType::IndexType destIndex;
  typename InputImageRegionType::SizeType  destSize;

  for (unsigned int i = 0; i < InputImageDimension; ++i)
  {
    destIndex[i] = srcIndex[i] * GetDecimationFactor();
    destSize[i]  = (srcSize[i] - 1) * GetDecimationFactor() + 1;
  }

  destRegion.SetIndex(destIndex);
  destRegion.SetSize(destSize);
}

template <class TInputImage, class TOutputImage>
void DecimateImageFilter<TInputImage, TOutputImage>::CallCopyInputRegionToOutputRegion(OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
  typename InputImageRegionType::SizeType  srcSize  = srcRegion.GetSize();

  typename OutputImageRegionType::IndexType destIndex;
  typename OutputImageRegionType::SizeType  destSize;

  for (unsigned int i = 0; i < InputImageDimension; ++i)
  {
    destIndex[i] = srcIndex[i] / GetDecimationFactor();
    destSize[i]  = (srcSize[i] - 1) / GetDecimationFactor() + 1;
  }

  destRegion.SetIndex(destIndex);
  destRegion.SetSize(destSize);
}

template <class TInputImage, class TOutputImage>
void DecimateImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                          itk::ThreadIdType itkNotUsed(threadId))
{
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  SubsampledImageRegionConstIterator<InputImageType> decimationIterator(this->GetInput(), inputRegionForThread);
  decimationIterator.SetSubsampleFactor(GetDecimationFactor());
  decimationIterator.GoToBegin();

  itk::ImageRegionIterator<InputImageType> outputIter(this->GetOutput(), outputRegionForThread);
  outputIter.GoToBegin();

  while (!decimationIterator.IsAtEnd() && !outputIter.IsAtEnd())
  {
    outputIter.Set(static_cast<OutputPixelType>(decimationIterator.Get()));

    ++outputIter;
    ++decimationIterator;
  }
}

} // end of namespace otb

#endif
