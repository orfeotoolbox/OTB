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

#ifndef otbLabelizeNeighborhoodConnectedImageFilter_hxx
#define otbLabelizeNeighborhoodConnectedImageFilter_hxx

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
