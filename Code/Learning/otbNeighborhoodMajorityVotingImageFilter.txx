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
#ifndef __otbNeighborhoodMajorityVotingImageFilter_txx
#define __otbNeighborhoodMajorityVotingImageFilter_txx

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"


#include "otbNeighborhoodMajorityVotingImageFilter.h"

namespace otb {

template<class TInputImage>
NeighborhoodMajorityVotingImageFilter<TInputImage>::NeighborhoodMajorityVotingImageFilter()
{
       this->SetNoDataValue(itk::NumericTraits<PixelType>::NonpositiveMin());
       this->SetUndefinedValue(itk::NumericTraits<PixelType>::NonpositiveMin());

       RadiusType radInit;
       radInit[0] = 1;
       radInit[1] = 1;
       this->SetRadiusNeighborhood(radInit);
}

template<class TInputImage>
typename NeighborhoodMajorityVotingImageFilter<TInputImage>::PixelType
              NeighborhoodMajorityVotingImageFilter<TInputImage>::Evaluate(const NeighborhoodIteratorType &nit,
              const KernelIteratorType kernelBegin,
              const KernelIteratorType kernelEnd) {

       /*
       KernelIteratorType kernelBegin_ = this->GetKernel().Begin();
       KernelIteratorType kernelEnd_ = this->GetKernel().End();
       */

       PixelType majorityLabel = 0; //Value of the more representative pixels in the neighborhood
       unsigned int majorityFreq = 0; //Number of pixels with the more representative value (majorityLabel) in the neighborhood

       unsigned int i;
       KernelIteratorType kernel_it;

       std::map<PixelType, unsigned int> histoNeigh;

       PixelType centerPixel = nit.GetCenterPixel();

       if (centerPixel != m_NoDataValue){
              for (i = 0, kernel_it = kernelBegin; kernel_it < kernelEnd;       ++kernel_it, ++i) {
                     // if structuring element is positive, use the pixel under that element
                     // in the image

                     PixelType label = nit.GetPixel(i);
                     if ((*kernel_it > itk::NumericTraits<KernelPixelType>::Zero) && (label != m_NoDataValue)) {
                            // note we use GetPixel() on the SmartNeighborhoodIterator to
                            // respect boundary conditions

                            //If the current label has already been added to the histogram histoNeigh
                            if(histoNeigh.count(label) > 0){
                                   histoNeigh[label]++;
                            }
                            else{
                                   histoNeigh[label] = 1;
                            }
                     }
              }

              //Extraction of the more representative Label in the neighborhood (majorityLabel) and its Frequency (majorityFreq)
              typename std::map<PixelType, unsigned int>::iterator histoIt;
              for (histoIt = histoNeigh.begin(); histoIt != histoNeigh.end();       ++histoIt) {
                     if (histoIt->second > majorityFreq) {
                            majorityFreq = histoIt->second; //Frequency
                            majorityLabel = histoIt->first; //Label
                     }
              }

              //If the majorityLabel is NOT unique in the neighborhood
              for (histoIt = histoNeigh.begin(); histoIt != histoNeigh.end();       ++histoIt) {
                     if ( (histoIt->second == majorityFreq) && (histoIt->first != majorityLabel) ) {
                            majorityLabel = m_UndefinedValue;
                            break;
                     }
              }
              
       }

       //If centerPixel == m_NoDataValue
       else{
              majorityLabel = m_UndefinedValue;
       }

       return majorityLabel;
}

} // end namespace otb

#endif
