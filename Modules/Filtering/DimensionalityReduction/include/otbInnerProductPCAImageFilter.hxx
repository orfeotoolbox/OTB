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

#ifndef otbInnerProductPCAImageFilter_hxx
#define otbInnerProductPCAImageFilter_hxx

#include "otbInnerProductPCAImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
InnerProductPCAImageFilter<TInputImage, TOutputImage>::InnerProductPCAImageFilter()
{
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageType::New());
  m_EstimatePCAFilter                   = EstimatePCAFilterType::New();
  m_NormalizePCAFilter                  = NormalizePCAFilterType::New();
  m_NumberOfPrincipalComponentsRequired = 1;
  m_CenterData                          = true;
  m_GenerateMeanComponent               = false;
  m_MeanFilter                          = MeanFilterType::New();
  m_CastFilter                          = CastFilterType::New();
  m_ConcatenateFilter                   = ConcatenateFilterType::New();
}
/**
 * GenerateOutputInformation
 */
template <class TInputImage, class TOutputImage>
void InnerProductPCAImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  if (m_GenerateMeanComponent == false)
    this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired);
  else
    this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired + 1);
}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage>
void InnerProductPCAImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  m_EstimatePCAFilter->SetInput(this->GetInput());
  m_EstimatePCAFilter->SetNumberOfPrincipalComponentsRequired(m_NumberOfPrincipalComponentsRequired);
  m_EstimatePCAFilter->SetCenterData(m_CenterData);

  m_NormalizePCAFilter->SetInput(m_EstimatePCAFilter->GetOutput());

  if ((m_CenterData == false) || ((m_CenterData == true) && (m_GenerateMeanComponent == false)))
  {
    m_NormalizePCAFilter->GraftOutput(this->GetOutput());
    m_NormalizePCAFilter->Update();
    this->GraftOutput(m_NormalizePCAFilter->GetOutput());
  }
  else
  {
    m_MeanFilter->SetInput(this->GetInput());
    m_CastFilter->SetInput(m_MeanFilter->GetOutput());

    m_ConcatenateFilter->SetInput1(m_NormalizePCAFilter->GetOutput());
    m_ConcatenateFilter->SetInput2(m_CastFilter->GetOutput());

    m_ConcatenateFilter->GraftOutput(this->GetOutput());
    m_ConcatenateFilter->Update();
    this->GraftOutput(m_ConcatenateFilter->GetOutput());
  }
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void InnerProductPCAImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
