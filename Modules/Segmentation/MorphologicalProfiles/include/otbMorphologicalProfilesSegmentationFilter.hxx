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

#ifndef otbMorphologicalProfilesSegmentationFilter_hxx
#define otbMorphologicalProfilesSegmentationFilter_hxx

#include "otbMorphologicalProfilesSegmentationFilter.h"

namespace otb
{

template <class TInputImage,  class TOutputImage, class TInternalPrecision, class TStructuringElement>
MorphologicalProfilesSegmentationFilter<TInputImage,TOutputImage,TInternalPrecision,TStructuringElement>
::MorphologicalProfilesSegmentationFilter()
{
  m_ProfileSize = 5;
  m_ProfileStart = 1;
  m_ProfileStep = 1;
  m_Sigma = 0.0;

  m_ClassificationFilter = MultiScaleClassificationFilterType::New();
  m_ConnectedComponentsFilter = ConnectedComponentsFilterType::New();
  m_OpeningProfile = OpeningProfileFilterType::New();
  m_ClosingProfile = ClosingProfileFilterType::New();
  m_OpeningDerivativeProfile = DerivativeFilterType::New();
  m_ClosingDerivativeProfile = DerivativeFilterType::New();
  m_OpeningCharacteristicsFilter = MultiScaleCharacteristicsFilterType::New();
  m_ClosingCharacteristicsFilter = MultiScaleCharacteristicsFilterType::New();
  // Wire pipeline
  m_OpeningDerivativeProfile->SetInput(m_OpeningProfile->GetOutput());
  m_ClosingDerivativeProfile->SetInput(m_ClosingProfile->GetOutput());
  m_OpeningCharacteristicsFilter->SetInput(m_OpeningDerivativeProfile->GetOutput());
  m_ClosingCharacteristicsFilter->SetInput(m_ClosingDerivativeProfile->GetOutput());

  using namespace Functor::MultiScaleConvexOrConcaveDecisionRule_tags;
  // Template keyword mandatory to avoid parsing error when using
  // template methods within template code
  m_ClassificationFilter->template SetVariadicNamedInput<max_opening_profile_derivative>(m_OpeningCharacteristicsFilter->GetOutput());
  m_ClassificationFilter->template SetVariadicNamedInput<opening_profile_characteristics>(m_OpeningCharacteristicsFilter->GetOutputCharacteristics());
  m_ClassificationFilter->template SetVariadicNamedInput<max_closing_profile_derivative>(m_ClosingCharacteristicsFilter->GetOutput());
  m_ClassificationFilter->template SetVariadicNamedInput<closing_profile_characteristics>(m_ClosingCharacteristicsFilter->GetOutputCharacteristics());

  m_ConnectedComponentsFilter->SetInput(m_ClassificationFilter->GetOutput());

}

template <class TInputImage,  class TOutputImage, class TInternalPrecision, class TStructuringElement>
MorphologicalProfilesSegmentationFilter<TInputImage,TOutputImage,TInternalPrecision,TStructuringElement>
::~MorphologicalProfilesSegmentationFilter()
{}

template <class TInputImage,  class TOutputImage, class TInternalPrecision, class TStructuringElement>
void
MorphologicalProfilesSegmentationFilter<TInputImage,TOutputImage,TInternalPrecision,TStructuringElement>
::GenerateData()
{
  m_OpeningProfile->SetInput(this->GetInput());
  m_ClosingProfile->SetInput(this->GetInput());

  m_OpeningProfile->SetProfileSize(m_ProfileSize);
  m_OpeningProfile->SetInitialValue(m_ProfileStart);
  m_OpeningProfile->SetStep(m_ProfileStep);

  m_ClosingProfile->SetProfileSize(m_ProfileSize);
  m_ClosingProfile->SetInitialValue(m_ProfileStart);
  m_ClosingProfile->SetStep(m_ProfileStep);

  m_OpeningCharacteristicsFilter->SetInitialValue(m_ProfileStart);
  m_OpeningCharacteristicsFilter->SetStep(m_ProfileStep);

  m_ClosingCharacteristicsFilter->SetInitialValue(m_ProfileStart);
  m_ClosingCharacteristicsFilter->SetStep(m_ProfileStep);

  m_ClassificationFilter->GetModifiableFunctor().SetSigma(m_Sigma);
  m_ClassificationFilter->GetModifiableFunctor().SetLabelSeparator(m_ProfileStart + m_ProfileSize * m_ProfileStep);

  m_ConnectedComponentsFilter->GraftOutput(this->GetOutput());
  m_ConnectedComponentsFilter->Update();
  this->GraftOutput(m_ConnectedComponentsFilter->GetOutput());
}

}
#endif
