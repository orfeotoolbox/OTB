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

  unsigned int i;
  KernelIteratorType kernel_it;

  std::map<PixelType, unsigned int> histoNeigh;

  PixelType centerPixel = nit.GetCenterPixel();

  if (centerPixel != m_LabelForNoDataPixels)
  {
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

    //Extraction of the more representative Label in the neighborhood (majorityLabel) and its Frequency (majorityFreq)
    typename std::map<PixelType, unsigned int>::iterator histoIt;
    for (histoIt = histoNeigh.begin(); histoIt != histoNeigh.end(); ++histoIt)
    {
      if (histoIt->second > majorityFreq)
      {
        majorityFreq = histoIt->second; //Frequency
        majorityLabel = histoIt->first; //Label
      }
    }

    //If the majorityLabel is NOT unique in the neighborhood
    for (histoIt = histoNeigh.begin(); histoIt != histoNeigh.end(); ++histoIt)
    {
      if ( (histoIt->second == majorityFreq) && (histoIt->first != majorityLabel) )
      {
         if (m_KeepOriginalLabelBool == true)
         {
           majorityLabel = centerPixel;
         }
         else
         {
           majorityLabel = m_LabelForUndecidedPixels;
         }
         break;
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

} // end namespace otb

#endif
