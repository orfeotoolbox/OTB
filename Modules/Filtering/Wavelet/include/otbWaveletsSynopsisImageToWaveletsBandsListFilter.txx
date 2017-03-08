/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbWaveletsSynopsisImageToWaveletsBandsListFilter_txx
#define otbWaveletsSynopsisImageToWaveletsBandsListFilter_txx

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

        unsigned int offsetX = largestSize[0]/(unsigned int)vcl_pow((double)m_DecimationRatio,(double)m_NumberOfLevels-currentLevel);
        unsigned int offsetY = largestSize[1]/(unsigned int)vcl_pow((double)m_DecimationRatio,(double)m_NumberOfLevels-currentLevel);

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
        currentSize[0] = largestSize[0]/(unsigned int)vcl_pow((double)m_DecimationRatio,(double)m_NumberOfLevels);
        currentSize[1] = largestSize[1]/(unsigned int)vcl_pow((double)m_DecimationRatio,(double)m_NumberOfLevels);
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
