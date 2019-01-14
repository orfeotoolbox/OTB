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

#ifndef otbHarrisImageToPointSetFilter_hxx
#define otbHarrisImageToPointSetFilter_hxx

#include "otbHarrisImageToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::HarrisImageToPointSetFilter()
{
  m_SigmaD = 1.0;
  m_SigmaI = 1.0;
  m_Alpha  = 1.0;

  m_LowerThreshold = itk::NumericTraits<InputPixelType>::NonpositiveMin();
  m_UpperThreshold = itk::NumericTraits<InputPixelType>::max();

  m_HarrisFilter     = HarrisImageFilterType::New();
  m_ThresholdFilter  = ThresholdImageToPointSetType::New();
}

template <class TInputImage, class TOutputPointSet>
void
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::GenerateData()
{

  typename OutputPointSetType::Pointer pointList = this->GetOutput();

  m_HarrisFilter->SetInput(0, this->GetInput(0));
  m_HarrisFilter->SetSigmaD(m_SigmaD);
  m_HarrisFilter->SetSigmaI(m_SigmaI);
  m_HarrisFilter->SetAlpha(m_Alpha);

  m_ThresholdFilter->SetInput(0, m_HarrisFilter->GetOutput());
  m_ThresholdFilter->SetLowerThreshold(m_LowerThreshold);
  m_ThresholdFilter->SetUpperThreshold(m_UpperThreshold);

  m_ThresholdFilter->SetOutput(pointList);
  m_ThresholdFilter->Update();

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputPointSet>
void
HarrisImageToPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Sigma_D    : " << this->m_SigmaD    << std::endl;
  os << indent << "Sigma_I    : " << this->m_SigmaI    << std::endl;
  os << indent << "Alpha      : " << this->m_Alpha     << std::endl;
  os << indent << "LowerThreshold  : " << this->m_LowerThreshold << std::endl;
  os << indent << "UpperThreshold  : " << this->m_UpperThreshold << std::endl;
}

} // end namespace otb

#endif
