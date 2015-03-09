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
#ifndef __otbLabelizeNeighborhoodConnectedImageFilter_txx
#define __otbLabelizeNeighborhoodConnectedImageFilter_txx

#include "otbLabelizeNeighborhoodConnectedImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template  <class TInputImage, class TOutputImage>
LabelizeNeighborhoodConnectedImageFilter<TInputImage, TOutputImage>
::LabelizeNeighborhoodConnectedImageFilter()
{
  m_LowerThresholdDelta = itk::NumericTraits<InputPixelType>::NonpositiveMin();
  m_UpperThresholdDelta = itk::NumericTraits<InputPixelType>::max();

  m_ReplaceValue = itk::NumericTraits<OutputPixelType>::One;
}

/** Region growing
 *
 */
template <class TInputImage, class TOutputImage>
void
LabelizeNeighborhoodConnectedImageFilter<TInputImage, TOutputImage>
::RegionGrowing(const IndexType indexSeed)
{
  InputPixelType threshold = this->GetInput()->GetPixel(indexSeed);

  this->m_RegionGrowingFilter->SetLower(threshold - m_LowerThresholdDelta);
  this->m_RegionGrowingFilter->SetUpper(threshold + m_UpperThresholdDelta);

  this->m_RegionGrowingFilter->SetReplaceValue(m_ReplaceValue);
  this->m_RegionGrowingFilter->SetSeed(indexSeed);
  m_ReplaceValue++;
}

/** PrintSelf Method
 *
 */
template <class TInputImage, class TOutputImage>
void
LabelizeNeighborhoodConnectedImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "LowerThresholdDelta: " << m_LowerThresholdDelta << std::endl;
  os << indent << "UpperThresholdDelta: " << m_UpperThresholdDelta << std::endl;
  os << indent << "ReplaceValue: " << m_ReplaceValue << std::endl;
}
} // end namespace otb

#endif
