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
#ifndef otbLabelizeConfidenceConnectedImageFilter_txx
#define otbLabelizeConfidenceConnectedImageFilter_txx

#include "itkNumericTraits.h"

#include "otbLabelizeConfidenceConnectedImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template  <class TInputImage, class TOutputImage>
LabelizeConfidenceConnectedImageFilter<TInputImage, TOutputImage>
::LabelizeConfidenceConnectedImageFilter()
{
  m_ReplaceValue = itk::NumericTraits<OutputPixelType>::One;
}

/** Region growing
 *
 */
template <class TInputImage, class TOutputImage>
void
LabelizeConfidenceConnectedImageFilter<TInputImage, TOutputImage>
::RegionGrowing(const IndexType indexSeed)
{
  this->m_RegionGrowingFilter->SetReplaceValue(m_ReplaceValue);
  this->m_RegionGrowingFilter->SetSeed(indexSeed);
  m_ReplaceValue++;
}

/** PrintSelf Method
 *
 */
template <class TInputImage, class TOutputImage>
void
LabelizeConfidenceConnectedImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "ReplaceValue: " << m_ReplaceValue << std::endl;
}
} // end namespace otb

#endif
