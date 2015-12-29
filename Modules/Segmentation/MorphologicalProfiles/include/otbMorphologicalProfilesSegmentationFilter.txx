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
#ifndef __otbMorphologicalProfilesSegmentationFilter_txx
#define __otbMorphologicalProfilesSegmentationFilter_txx

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

  m_ClassificationFilter->SetOpeningProfileDerivativeMaxima(m_OpeningCharacteristicsFilter->GetOutput());
  m_ClassificationFilter->SetOpeningProfileCharacteristics(m_OpeningCharacteristicsFilter->GetOutputCharacteristics());
  m_ClassificationFilter->SetClosingProfileDerivativeMaxima(m_ClosingCharacteristicsFilter->GetOutput());
  m_ClassificationFilter->SetClosingProfileCharacteristics(m_ClosingCharacteristicsFilter->GetOutputCharacteristics());

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

  m_ClassificationFilter->SetSigma(m_Sigma);
  m_ClassificationFilter->SetLabelSeparator(m_ProfileStart + m_ProfileSize * m_ProfileStep);

  m_ConnectedComponentsFilter->GraftOutput(this->GetOutput());
  m_ConnectedComponentsFilter->Update();
  this->GraftOutput(m_ConnectedComponentsFilter->GetOutput());
}

}
#endif
