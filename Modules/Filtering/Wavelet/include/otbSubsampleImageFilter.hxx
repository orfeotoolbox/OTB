/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbSubsampleImageFilter_hxx
#define otbSubsampleImageFilter_hxx
#include "otbSubsampleImageFilter.h"

#include "otbMacro.h"
#include "otbSubsampledImageRegionIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "SubsampleFactor = [" << m_SubsampleFactor[0];
  for (unsigned int i = 1; i < InputImageDimension; ++i)
  {
    os << ", " << m_SubsampleFactor[i];
  }
  os << "]\n";
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
bool SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::IsSubsampleFactorOne() const
{
  for (unsigned int i = 0; i < InputImageDimension; ++i)
  {
    if (m_SubsampleFactor[i] != 1)
      return false;
  }

  return true;
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if (!IsSubsampleFactorOne())
  {
    OutputImageRegionType newRegion;
    this->CallCopyInputRegionToOutputRegion(newRegion, this->GetInput()->GetLargestPossibleRegion());
    this->GetOutput()->SetRegions(newRegion);
  }
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::CallCopyOutputRegionToInputRegion(InputImageRegionType& destRegion,
                                                                                                                    const OutputImageRegionType& srcRegion)
{
  Superclass::CallCopyOutputRegionToInputRegion(destRegion, srcRegion);

  if (DirectionOfTransformation == Wavelet::INVERSE)
  {
    typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename OutputImageRegionType::SizeType  srcSize  = srcRegion.GetSize();

    typename InputImageRegionType::IndexType destIndex;
    typename InputImageRegionType::SizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
      // TODO: This seems not right in odd index cases
      destIndex[i] = srcIndex[i] / m_SubsampleFactor[i];
      destSize[i]  = srcSize[i] / m_SubsampleFactor[i];
    }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
  }
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::CallCopyInputRegionToOutputRegion(OutputImageRegionType& destRegion,
                                                                                                                    const InputImageRegionType& srcRegion)
{
  Superclass::CallCopyInputRegionToOutputRegion(destRegion, srcRegion);

  if (DirectionOfTransformation == Wavelet::INVERSE)
  {
    typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
    typename InputImageRegionType::SizeType  srcSize  = srcRegion.GetSize();

    typename OutputImageRegionType::IndexType destIndex;
    typename OutputImageRegionType::SizeType  destSize;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
      destIndex[i] = srcIndex[i] * m_SubsampleFactor[i];
      destSize[i]  = srcSize[i] * m_SubsampleFactor[i];
    }

    destRegion.SetIndex(destIndex);
    destRegion.SetSize(destSize);
  }
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::BeforeThreadedGenerateData()
{
  OutputImagePointerType output = this->GetOutput();
  output->FillBuffer(0);
}

template <class TInputImage, class TOutputImage, Wavelet::WaveletDirection TDirectionOfTransformation>
void SubsampleImageFilter<TInputImage, TOutputImage, TDirectionOfTransformation>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                                       itk::ThreadIdType itkNotUsed(threadId))
{
  OutputImagePointerType output = this->GetOutput();

  SubsampledImageRegionIterator<OutputImageType> outputIter(this->GetOutput(), outputRegionForThread);
  outputIter.SetSubsampleFactor(1);
  outputIter.GoToBegin();

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  SubsampledImageRegionConstIterator<InputImageType> inputIter(this->GetInput(), inputRegionForThread);

  if (DirectionOfTransformation == Wavelet::FORWARD)
  {
    inputIter.SetSubsampleFactor(GetSubsampleFactor());
    inputIter.GoToBegin();

    while (!inputIter.IsAtEnd())
    {
      outputIter.SetOffset(static_cast<typename SubsampledImageRegionIterator<OutputImageType>::OffsetType>(inputIter.GetOffset()));
      outputIter.Set(static_cast<OutputPixelType>(inputIter.Get()));
      ++inputIter;
    }
  }
  else
  {
    inputIter.SetSubsampleFactor(1);
    inputIter.GoToBegin();

    while (!inputIter.IsAtEnd())
    {
      InputImageIndexType  inputIndex = inputIter.GetIndex();
      OutputImageIndexType outputIndex;
      for (unsigned int i = 0; i < OutputImageDimension; ++i)
      {
        outputIndex[i] = inputIndex[i] * m_SubsampleFactor[i];
      }
      outputIter.SetIndex(outputIndex);
      outputIter.Set(static_cast<OutputPixelType>(inputIter.Get()));
      ++inputIter;
    }
  }
}

} // end of namespace otb

#endif
