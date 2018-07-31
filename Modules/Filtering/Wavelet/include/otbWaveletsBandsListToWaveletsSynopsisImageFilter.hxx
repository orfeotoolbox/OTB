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

#ifndef otbWaveletsBandsListToWaveletsSynopsisImageFilter_hxx
#define otbWaveletsBandsListToWaveletsSynopsisImageFilter_hxx

#include "otbWaveletsBandsListToWaveletsSynopsisImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{
/** Constructor */
template <class TImageList, class TImage>
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageList,TImage>
::WaveletsBandsListToWaveletsSynopsisImageFilter() : m_DecimationRatio(2)
{
OTB_DISABLE_DYNAMIC_MT
}

/** Destructor */
template <class TImageList, class TImage>
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageList,TImage>
::~WaveletsBandsListToWaveletsSynopsisImageFilter()
{}

/**
 * GenerateOutputInformation
 */
template <class TImageList, class TImage>
void
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageList,TImage>
::GenerateOutputInformation(void)
{
  // We must set the size of the output image to be twice the size of the last image
  // of the image list, which is the first band.
  if(this->GetOutput())
  {
    if (this->GetInput()->Size()>0)
    {
    // Retrieve the largest band
    typename InputImageType::Pointer lastBand = this->GetInput()->Back();

    // Retrieve the region of the largest band
    RegionType largestBandRegion = lastBand->GetLargestPossibleRegion();

    // Retrieve the size of the largest region
    typename RegionType::SizeType outputSize = largestBandRegion.GetSize();

    // Multiply this size by two
    outputSize[0]*=m_DecimationRatio;
    outputSize[1]*=m_DecimationRatio;

    // Build the output region
    RegionType outputLargestRegion;
    outputLargestRegion.SetSize(outputSize);

    // Copy information to the output image
    this->GetOutput()->CopyInformation(lastBand);
    this->GetOutput()->SetLargestPossibleRegion(outputLargestRegion);

    }
  }
}
/**
 * GenerateInputRequestedRegion
 */
template <class TImageList, class TImage>
void
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageList,TImage>
::GenerateInputRequestedRegion(void)
{
  typename InputImageListType::Pointer inputPtr = this->GetInput();
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  while (inputListIt!=inputPtr->End())
    {
      inputListIt.Get()->SetRequestedRegionToLargestPossibleRegion();
      ++inputListIt;
    }
}

/**
 * Main computation method
 */
template <class TImageList, class TImage>
void
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageList,TImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  // Retrieve input and output pointers
  typename InputImageListType::Pointer inputPtr = this->GetInput();
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  // Set up progress reporting
  itk::ProgressReporter progress(this,
                                 threadId,
                                 outputRegionForThread.GetNumberOfPixels());

  // defines input and output iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  // Set up an iterator on the input wavelets band
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  unsigned int bandIndex = 0;

  // Compute number of decomposition levels
  unsigned int numberOfDecompositionLevels = (inputPtr->Size()-1)/3;

  // Retrieve the largest possible region size
  typename RegionType::SizeType largestSize = outputPtr->GetLargestPossibleRegion().GetSize();

  // Iterate on each band
  for (;inputListIt!=inputPtr->End();++inputListIt,++bandIndex)
  {
  // Build a band offset
  typename RegionType::OffsetType currentOffset;
  currentOffset.Fill(0);

  // Initialise Current level
  unsigned int currentLevel = 0;
  unsigned int currentSubBand = 0;

  if(bandIndex > 0)
    {
    // Compute current level and sub band
    currentLevel = 1 + (bandIndex-1)/3;
    currentSubBand = (bandIndex-1)%3;

    // Compute potentiel offset in x and y
    unsigned int offsetX = largestSize[0]/(unsigned int)std::pow((double)m_DecimationRatio,(double)1+numberOfDecompositionLevels-currentLevel);
    unsigned int offsetY = largestSize[1]/(unsigned int)std::pow((double)m_DecimationRatio,(double)1+numberOfDecompositionLevels-currentLevel);

    // Compute final offset according to the subband index
    if(currentSubBand == 0)
      {
      currentOffset[0]+=offsetX;
      }
    else if(currentSubBand == 1)
      {
      currentOffset[1]+=offsetY;
      }
    else
      {
      currentOffset[0]+=offsetX;
      currentOffset[1]+=offsetY;
      }
    }
    // Retrieve current band region
    RegionType currentBandRegion = inputListIt.Get()->GetLargestPossibleRegion();

    // Apply offset to get the current output region
    RegionType currentOutputRegion = currentBandRegion;
    typename RegionType::IndexType currentOutputIndex = currentBandRegion.GetIndex();
    currentOutputIndex+=currentOffset;
    currentOutputRegion.SetIndex(currentOutputIndex);

    // Crop with the outputRegionForThread. If the crop fails,
    // it means that currentOutputRegion is outside of outputRegionForThread,
    // and in this case we skip to the next image in the list.
    if(currentOutputRegion.Crop(outputRegionForThread))
      {
      // Compute the corresponding input region
      RegionType currentInputRegion = currentBandRegion;
      currentOutputIndex = currentOutputRegion.GetIndex();
      typename RegionType::IndexType currentInputIndex = currentBandRegion.GetIndex();

      for(unsigned int i = 0; i < InputImageType::ImageDimension;++i)
        {
        currentInputIndex[i]+=currentOutputIndex[i];
        currentInputIndex[i]-=currentOffset[i];
        }
        currentInputRegion.SetSize(currentOutputRegion.GetSize());
        currentInputRegion.SetIndex(currentInputIndex);

      InputIteratorType inIt(inputListIt.Get(),currentInputRegion);
      OutputIteratorType outIt(outputPtr,currentOutputRegion);

      // Go to begin
      inIt.GoToBegin();
      outIt.GoToBegin();

      // Copy pixels
      while(!inIt.IsAtEnd() && !outIt.IsAtEnd())
      {
        // Copy pixel value
        outIt.Set(static_cast<typename OutputImageType::InternalPixelType>(inIt.Get()));
        // Step forward
        ++inIt;
        ++outIt;
        progress.CompletedPixel();
      }
    }
  }
}
/**
 * PrintSelf Method
 */
template <class TImageType, class TImageList>
void
WaveletsBandsListToWaveletsSynopsisImageFilter<TImageType,TImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
