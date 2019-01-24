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

#ifndef otbWaveletsSynopsisImageToWaveletsBandsListFilter_hxx
#define otbWaveletsSynopsisImageToWaveletsBandsListFilter_hxx

#include "otbWaveletsSynopsisImageToWaveletsBandsListFilter.h"

namespace otb
{
/** Constructor */
template <class TImage, class TImageList>
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::WaveletsSynopsisImageToWaveletsBandsListFilter() : m_NumberOfLevels(1),
                                                     m_DecimationRatio(2),
                                                     m_ExtractFilters(),
                                                     m_ExtractFiltersUpToDate(false)
{}

/** Destructor */
template <class TImage, class TImageList>
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::~WaveletsSynopsisImageToWaveletsBandsListFilter()
{
  m_ExtractFilters.clear();
}

/** Modify overload */
template <class TImage, class TImageList>
void
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::Modified() const
{
  // Call superclass implementation
  Superclass::Modified();
  m_ExtractFiltersUpToDate = false;
}

/** Generate the input requested region from the first element in the list. */
template <class TImage, class TImageList>
void
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::GenerateOutputInformation(void)
{
  typename OutputImageListType::Pointer outputPtr = this->GetOutput();
  typename InputImageType::ConstPointer inputPtr = this->GetInput();

  // Check if we need to regenerate the extract filters
  if (inputPtr && !m_ExtractFiltersUpToDate)
  {
    // Retrieve image size
    typename RegionType::SizeType largestSize = inputPtr->GetLargestPossibleRegion().GetSize();

    // Compute the number of output images
    unsigned int numberOfOutputImages = m_NumberOfLevels*3 + 1;

    // Clear the output image list
    outputPtr->Clear();

    // Clear the extract filter vector
    m_ExtractFilters.clear();

    // For each output image
    for (unsigned int i=0;i<numberOfOutputImages;++i)
      {
      // Build the current extract filter
      typename ExtractFilterType::Pointer currentExtract = ExtractFilterType::New();
      currentExtract->SetInput(inputPtr);

      // Add it to the filter list
      m_ExtractFilters.push_back(currentExtract);

      // Add its output to the filter's output
      outputPtr->PushBack(currentExtract->GetOutput());

      // Build the corresponding region
      RegionType currentRegion;
      typename RegionType::IndexType currentIndex;
      typename RegionType::SizeType  currentSize;
      currentIndex.Fill(0);

      // If this is not the first sub-band
      if(i > 0)
        {
        // Compute current sub-band and level
        unsigned int currentLevel =   (i-1)/3;
        unsigned int currentSubBand = (i-1)%3;

        unsigned int offsetX = largestSize[0]/(unsigned int)std::pow((double)m_DecimationRatio,(double)m_NumberOfLevels-currentLevel);
        unsigned int offsetY = largestSize[1]/(unsigned int)std::pow((double)m_DecimationRatio,(double)m_NumberOfLevels-currentLevel);

        // Compute current size
        currentSize[0] = offsetX;
        currentSize[1] = offsetY;

        // Compute current index
        if(currentSubBand == 0)
          {
           currentIndex[0]+= offsetX;
          }
        else if(currentSubBand == 1)
          {
           currentIndex[1]= offsetY;
          }
        else
          {
          currentIndex[0]= offsetX;
          currentIndex[1]= offsetY;
          }
        }
      else
        {
        // The coarsest scale size
        currentSize[0] = largestSize[0]/(unsigned int)std::pow((double)m_DecimationRatio,(double)m_NumberOfLevels);
        currentSize[1] = largestSize[1]/(unsigned int)std::pow((double)m_DecimationRatio,(double)m_NumberOfLevels);
        }
      // Build current region
      currentRegion.SetIndex(currentIndex);
      currentRegion.SetSize(currentSize);
      //std::cout<<"Band: "<<i<<", region "<<currentRegion<<std::endl;
      currentExtract->SetRegionOfInterest(currentRegion);
    }
    m_ExtractFiltersUpToDate = true;
  }
}
/** Generate the output information by building the output list. */
template <class TImage, class TImageList>
void
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::GenerateInputRequestedRegion(void)
{
  typename InputImageType::Pointer inputPtr = this->GetInput();

  if (inputPtr)
  {
  inputPtr->SetRequestedRegionToLargestPossibleRegion();
  }
}
/**
 * Main computation method
 */
template <class TImage, class TImageList>
void
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::GenerateData(void)
{
  // Update each extract fillter
  for(typename ExtractFilterVectorType::iterator it = m_ExtractFilters.begin();
      it!=m_ExtractFilters.end();++it)
    {
    (*it)->Update();
    }
}
/**
 * PrintSelf Method
 */
template <class TImage, class TImageList>
void
WaveletsSynopsisImageToWaveletsBandsListFilter<TImage,TImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
