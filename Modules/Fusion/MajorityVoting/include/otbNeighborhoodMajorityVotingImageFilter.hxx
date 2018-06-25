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

#ifndef otbNeighborhoodMajorityVotingImageFilter_hxx
#define otbNeighborhoodMajorityVotingImageFilter_hxx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.

#include "otbNeighborhoodMajorityVotingImageFilter.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{
/**
 * Constructor
 */
template<class TInputImage, class TOutputImage, class TKernel>
NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage, TKernel>::NeighborhoodMajorityVotingImageFilter()
{
  this->SetLabelForNoDataPixels(itk::NumericTraits<PixelType>::NonpositiveMin()); //m_LabelForNoDataPixels = 0
  this->SetLabelForUndecidedPixels(itk::NumericTraits<PixelType>::NonpositiveMin()); //m_LabelForUndecidedPixels = 0
  this->SetKeepOriginalLabelBool(true); //m_KeepOriginalLabelBool = true
  this->SetOnlyIsolatedPixels(false); //process all pixels 
  this->SetIsolatedThreshold(1);
}


template<class TInputImage, class TOutputImage, class TKernel>
typename NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage,
                                               TKernel>::PixelType 
NeighborhoodMajorityVotingImageFilter<TInputImage, 
                                      TOutputImage, TKernel>::Evaluate(const NeighborhoodIteratorType &nit,
                                                                       const KernelIteratorType kernelBegin,
                                                                       const KernelIteratorType kernelEnd) 
{
  const PixelType centerPixel = nit.GetCenterPixel();
  if (centerPixel == m_LabelForNoDataPixels)
    {
    return m_LabelForNoDataPixels;
    }
  else
    {
    //Get a histogram of label frequencies where the 2 highest are at the beginning and sorted
    const HistoSummary histoSummary = 
      this->ComputeNeighborhoodHistogramSummary(nit, kernelBegin, kernelEnd);
    if(m_OnlyIsolatedPixels && 
       histoSummary.freqCenterLabel > m_IsolatedThreshold)
      {
      //If we want to filter only isolated pixels, keep the label if
      //there are enough pixels with the center label to consider that
      //it is not isolated
      return centerPixel;
      }
    else
      {
      //If the majorityLabel is NOT unique in the neighborhood
      if(!histoSummary.majorityUnique)
        {
        if (m_KeepOriginalLabelBool == true)
          {
          return centerPixel;
          }
        else
          {
          return m_LabelForUndecidedPixels;
          }
        }
      //Extraction of the more representative Label in the neighborhood (majorityLabel)
      return histoSummary.majorityLabel;
      }
    }//END if (centerPixel != m_LabelForNoDataPixels)
}

template<class TInputImage, class TOutputImage, class TKernel>
const typename NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage,
                                                     TKernel>::HistoSummary 
NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage, 
                                      TKernel>::ComputeNeighborhoodHistogramSummary(const NeighborhoodIteratorType &nit,
                                                                          const KernelIteratorType kernelBegin,
                                                                          const KernelIteratorType kernelEnd) const
{  
  typedef std::map<PixelType, unsigned int> HistogramType;
  typedef std::vector<std::pair<PixelType, unsigned int> > HistoAsVectorType;
  HistogramType histoNeigh;
  unsigned int i = 0; 
  for (KernelIteratorType kernel_it = kernelBegin; 
       kernel_it < kernelEnd; ++kernel_it, ++i)
    {
    // if structuring element is positive, use the pixel under that element
    // in the image
    // note we use GetPixel() on the SmartNeighborhoodIterator to
    // respect boundary conditions
    const PixelType label = nit.GetPixel(i);
    if ((*kernel_it > itk::NumericTraits<KernelPixelType>::Zero) && 
        (label != m_LabelForNoDataPixels))
      {
      histoNeigh[label] += 1;
      }
    }
  typename NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage,
                                                 TKernel>::HistoSummary result;
  assert(!histoNeigh.empty());
  if (histoNeigh.size() == 1)
    {
    result.freqCenterLabel = histoNeigh.begin()->second;
    result.majorityLabel = histoNeigh.begin()->first;
    result.majorityUnique = true;
    }
  else
    {
    HistoAsVectorType histoNeighVec(histoNeigh.begin(), histoNeigh.end());
    //Sort the 2 max elements to the beginning
    std::nth_element(histoNeighVec.begin(), histoNeighVec.begin()+1,
                     histoNeighVec.end(), CompareHistoFequencies());
    result.freqCenterLabel = histoNeigh[nit.GetCenterPixel()];
    result.majorityLabel = histoNeighVec[0].first;
    result.majorityUnique = (histoNeighVec[0].second != histoNeighVec[1].second);
    }
  return result;
}

template<class TInputImage, class TOutputImage, class TKernel>
void
NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage, TKernel>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  TOutputImage* outputPtr = this->GetOutput();

  // Set the NoData value using the background
  const unsigned int & nbBands =  outputPtr->GetNumberOfComponentsPerPixel();
  std::vector<bool> noDataValueAvailable;
  noDataValueAvailable.resize(nbBands,true);
  std::vector<double> noDataValue;
  noDataValue.resize(nbBands,0.0);
  for (unsigned int i=0 ; i<nbBands ; ++i)
    {
    noDataValue[i] = itk::DefaultConvertPixelTraits<PixelType>::GetNthComponent(i,m_LabelForNoDataPixels);
    }
  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,noDataValueAvailable);
  itk::EncapsulateMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,noDataValue);
}

} // end namespace otb

#endif
