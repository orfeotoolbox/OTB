/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef otbNeighborhoodMajorityVotingImageFilter_txx
#define otbNeighborhoodMajorityVotingImageFilter_txx

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
  PixelType result = centerPixel; 
  if (centerPixel != m_LabelForNoDataPixels)
    {
    //Get a histogram of label frequencies where the 2 highest are at the beginning and sorted
    const HistoSummary histoSummary = this->ComputeNeighborhoodHistogramSummary(nit, kernelBegin, kernelEnd);

    if(m_OnlyIsolatedPixels && histoSummary.freqCenterLabel > m_IsolatedThreshold)
      {
      //If we want to filter only isolated pixels, keep the label if
      //there are enough pixels with the center label to consider that
      //it is not isolated
      result = centerPixel;
      }
    else
      {
      //Extraction of the more representative Label in the neighborhood (majorityLabel)
      result = histoSummary.majorityLabel;
      //If the majorityLabel is NOT unique in the neighborhood
      if(histoSummary.secondFreq == histoSummary.majorityFreq && histoSummary.secondLabel != histoSummary.majorityLabel)
        {
        if (m_KeepOriginalLabelBool == true)
          {
          result = centerPixel;
          }
        else
          {
          result = m_LabelForUndecidedPixels;
          }
        }
      }
    }//END if (centerPixel != m_LabelForNoDataPixels)
  //If (centerPixel == m_LabelForNoDataPixels)
  else
    {
    result = m_LabelForNoDataPixels;
    }
  return result;
}

template<class TInputImage, class TOutputImage, class TKernel>
const typename NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage,
                                                     TKernel>::HistoSummary 
NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage, 
                                      TKernel>::ComputeNeighborhoodHistogramSummary(const NeighborhoodIteratorType &nit,
                                                                          const KernelIteratorType kernelBegin,
                                                                          const KernelIteratorType kernelEnd) const
{  
  std::map<PixelType, unsigned int> histoNeigh;
  PixelType centerPixel = nit.GetCenterPixel();
  unsigned int i = 0; 
  for (KernelIteratorType kernel_it = kernelBegin; 
       kernel_it < kernelEnd; ++kernel_it, ++i)
    {
    // if structuring element is positive, use the pixel under that element
    // in the image
    // note we use GetPixel() on the SmartNeighborhoodIterator to
    // respect boundary conditions
    PixelType label = nit.GetPixel(i);
    if ((*kernel_it > itk::NumericTraits<KernelPixelType>::Zero) && (label != m_LabelForNoDataPixels))
      {
      //If the current label has already been added to the histogram histoNeigh
      histoNeigh[label] += 1;
      }
    }
  std::vector< std::pair<PixelType, unsigned int> > histoNeighVec;
  std::copy(histoNeigh.begin(), histoNeigh.end(), std::back_inserter(histoNeighVec));
  const typename std::vector<std::pair<PixelType, unsigned int> >::iterator histoIt = histoNeighVec.begin();
  std::nth_element(histoNeighVec.begin(), histoIt+1,
                   histoNeighVec.end(), CompareHistoFequencies());
  typename NeighborhoodMajorityVotingImageFilter<TInputImage, TOutputImage,
                                                 TKernel>::HistoSummary result;
  result.freqCenterLabel = histoNeigh[centerPixel];
  result.majorityFreq = histoNeighVec[0].second;
  result.majorityLabel = histoNeighVec[0].first;
  result.secondFreq = histoNeighVec[1].second;
  result.secondLabel = histoNeighVec[1].first;
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
