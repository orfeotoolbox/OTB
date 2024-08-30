/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbAsymmetricFusionOfLineDetectorImageFilter_hxx
#define otbAsymmetricFusionOfLineDetectorImageFilter_hxx

#include "otbAsymmetricFusionOfLineDetectorImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
AsymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::AsymmetricFusionOfLineDetectorImageFilter()
{
  this->m_Radius.Fill(1);
  this->m_LengthLine         = 1;
  this->m_WidthLine          = 0;
  this->m_NumberOfDirections = 8;

  m_LineRatio       = LineRatioType::New();
  m_LineCorrelation = LineCorrelationType::New();
  m_AssSymSum       = AssSymSumType::New();
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void AsymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::GenerateData()
{
  m_LineRatio->SetInput(this->GetInput());
  m_LineRatio->SetLengthLine(this->m_LengthLine);
  m_LineRatio->SetWidthLine(this->m_WidthLine);
  m_LineRatio->SetNumberOfDirections(this->m_NumberOfDirections);

  m_LineCorrelation->SetInput(this->GetInput());
  m_LineCorrelation->SetLengthLine(this->m_LengthLine);
  m_LineCorrelation->SetWidthLine(this->m_WidthLine);
  m_LineCorrelation->SetNumberOfDirections(this->m_NumberOfDirections);

  m_AssSymSum->SetInput1(m_LineRatio->GetOutput());
  m_AssSymSum->SetInput2(m_LineCorrelation->GetOutput());

  m_AssSymSum->GraftOutput(this->GetOutput());
  m_AssSymSum->Update();
  this->GraftOutput(m_AssSymSum->GetOutput());
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection, class TInterpolator>
void AsymmetricFusionOfLineDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>::PrintSelf(std::ostream& os,
                                                                                                                           itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // namespace otb
#endif
