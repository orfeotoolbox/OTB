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
                                               TKernel>::PixelType NeighborhoodMajorityVotingImageFilter<TInputImage,
                                                                                                         TOutputImage, TKernel>::Evaluate(const NeighborhoodIteratorType &nit,
                                                                                                                                          const KernelIteratorType kernelBegin,
                                                                                                                                          const KernelIteratorType kernelEnd)
{
  PixelType majorityLabel = 0; //Value of the more representative pixels in the neighborhood
  unsigned int majorityFreq = 0; //Number of pixels with the more representative value (majorityLabel) in the neighborhood
  
  PixelType centerPixel = nit.GetCenterPixel();

  if (centerPixel != m_LabelForNoDataPixels)
    {
    std::vector< std::pair<PixelType, unsigned int> > histoNeighVec;
    //Get a histogram of label frequencies where the 2 highest are at the beginning and sorted
    unsigned int freqCenterLabel = this->FillNeighborhoodHistogram(histoNeighVec, nit, kernelBegin, kernelEnd);

    if(m_OnlyIsolatedPixels && freqCenterLabel < m_IsolatedThreshold)
      {
      //If we want to filter only isolated pixels, keep the label if
      //there are enough pixels with the center label to consider that
      //it is not isolated
      majorityLabel = centerPixel;
      }
    else
      {
      //Extraction of the more representative Label in the neighborhood (majorityLabel) and its Frequency (majorityFreq)
      majorityFreq = histoNeighVec[0].second; //Frequency
      majorityLabel = histoNeighVec[0].first; //Label
      //If the majorityLabel is NOT unique in the neighborhood
      if(histoNeighVec[1].second == majorityFreq && histoNeighVec[1].first != majorityLabel)
        {
        if (m_KeepOriginalLabelBool == true)
          {
          majorityLabel = centerPixel;
          }
        else
          {
          majorityLabel = m_LabelForUndecidedPixels;
          }
        }
      }

    }//END if (centerPixel != m_LabelForNoDataPixels)

//If (centerPixel == m_LabelForNoDataPixels)
else
  {
  majorityLabel = m_LabelForNoDataPixels;
  }

return majorityLabel;
}

template<class TInputImage, class TOutputImage, class TKernel>
unsigned int NeighborhoodMajorityVotingImageFilter<TInputImage,
                                                   TOutputImage, 
                                                   TKernel>::FillNeighborhoodHistogram(std::vector<std::pair<PixelType, unsigned int> >& histoNeighVec, 
                                                                                       const NeighborhoodIteratorType &nit,
                                                                                       const KernelIteratorType kernelBegin,
                                                                                       const KernelIteratorType kernelEnd)
{  
  std::map<PixelType, unsigned int> histoNeigh;
  PixelType centerPixel = nit.GetCenterPixel();
  unsigned int i;
  KernelIteratorType kernel_it;
  for (i = 0, kernel_it = kernelBegin; kernel_it < kernelEnd; ++kernel_it, ++i)
    {
    // if structuring element is positive, use the pixel under that element
    // in the image

    PixelType label = nit.GetPixel(i);
    if ((*kernel_it > itk::NumericTraits<KernelPixelType>::Zero) && (label != m_LabelForNoDataPixels))
      {
      // note we use GetPixel() on the SmartNeighborhoodIterator to
      // respect boundary conditions

      //If the current label has already been added to the histogram histoNeigh
      if (histoNeigh.count(label) > 0)
        {
        histoNeigh[label]++;
        }
      else
        {
        histoNeigh[label] = 1;
        }
      }
    }
  std::copy(histoNeigh.begin(), histoNeigh.end(), std::back_inserter(histoNeighVec));
  typename std::vector<std::pair<PixelType, unsigned int> >::iterator histoIt = histoNeighVec.begin();
  std::nth_element(histoNeighVec.begin(), histoIt+1,
                   histoNeighVec.end(), CompareHistoFequencies());

  return histoNeigh[centerPixel];
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
