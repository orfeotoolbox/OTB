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

#ifndef otbAtmosphericEffects_hxx
#define otbAtmosphericEffects_hxx

#include "otbAtmosphericEffects.h"
#include "otbSurfaceReflectanceToReflectanceFilter.h"

namespace otb
{

template <class TSpectralResponse, class TRSR>
AtmosphericEffects<TSpectralResponse, TRSR>::AtmosphericEffects() : m_IsSetAtmosphericRadiativeTerms(false)
{
  m_InputSpectralResponse     = InputSpectralResponseType::New();
  m_CorrectedSpectralResponse = InputSpectralResponseType::New();
  m_InputSatRSR               = InputRSRType::New();
}
/*
template <class TSpectralResponse , class TRSR>
void
AtmosphericEffects<TSpectralResponse , TRSR>
::LoadFilterFunctionAtmosphericCorrectionParameters(double step)
{
  //LMoad the filter function from the RSR
  for (unsigned int i=0; i<this->m_InputSatRSR->GetNbBands(); ++i)
  {
    m_DataAtmosphericCorrectionParameters->SetWavelenghtSpectralBandWithIndex(i, this->m_InputSatRSR->GetRSR()[i]->GetFilterFunctionValues (step));
  }
}
*/
template <class TSpectralResponse, class TRSR>
void AtmosphericEffects<TSpectralResponse, TRSR>::Process(/*const unsigned int numBand*/)
{
  /*typedef otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
      AtmosphericCorrectionParametersTo6SRadiativeTermsType;

  AtmosphericCorrectionParametersTo6SRadiativeTermsType::Pointer  filterAtmosphericCorrectionParametersTo6SRadiativeTerms =
  AtmosphericCorrectionParametersTo6SRadiativeTermsType::New();

    //Set filter function to get the RSR Sat!!!//TODO
  typedef otb::FilterFunctionValues FilterFunctionValuesType;
  typedef FilterFunctionValuesType::ValuesVectorType ValuesVectorType;
  ValuesVectorType valuesVector;
  const double step = 0.0025;
  for (unsigned int i=0; i < this->m_InputSatRSR->GetNbBands(); ++i)
  {
    //Foreach band
    valuesVector.clear();
    PairType inter;
    inter = (this->m_InputSatRSR->GetRSR())[i]->GetInterval();
    otbGenericMsgDebugMacro(<<"inter " << inter.first << " " << inter.second);

    PairType pair;
    pair.first=(inter.first+inter.second)/2.0;
    pair.second=0.;
    m_CorrectedSpectralResponse->GetResponse().push_back(pair);

    for ( double j=inter.first; j <= inter.second; j+=step)
    {
      valuesVector.push_back( (*this->m_InputSatRSR->GetRSR()[i])(j) );
    }

    FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
    functionValues->SetFilterFunctionValues(valuesVector);
    functionValues->SetMinSpectralValue(inter.first);
    functionValues->SetMaxSpectralValue(inter.second - step);

  //TODO Is it the efficient method Problem in 6STraits
    functionValues->SetUserStep( step );

    m_DataAtmosphericCorrectionParameters->SetWavelengthSpectralBandWithIndex(i, functionValues);
  }

  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->SetInput( m_DataAtmosphericCorrectionParameters );
  filterAtmosphericCorrectionParametersTo6SRadiativeTerms->Update();

  otbGenericMsgDebugMacro(<<"6S radiative term performed");

  typedef typename InputSpectralResponseType::ImageType       ImageType;

  typedef SurfaceReflectanceToReflectanceFilter<ImageType, ImageType>              SurfaceReflectanceToReflectanceFilterType;
  typename SurfaceReflectanceToReflectanceFilterType::Pointer filterSRToR = SurfaceReflectanceToReflectanceFilterType::New();

  filterSRToR->SetAtmosphericRadiativeTerms( filterAtmosphericCorrectionParametersTo6SRadiativeTerms->GetOutput() );

  typename ImageType::Pointer image = ImageType::New();
  this->m_InputSpectralResponse->GetImage(image);
  filterSRToR->SetInput( image );
  filterSRToR->Update();


  this->m_CorrectedSpectralResponse->SetFromImage( filterSRToR->GetOutput() ); */

  /*if (m_IsSetAtmosphericRadiativeTerms)
  {*/
  typedef typename InputSpectralResponseType::ImageType ImageType;

  typedef SurfaceReflectanceToReflectanceFilter<ImageType, ImageType> SurfaceReflectanceToReflectanceFilterType;
  typename SurfaceReflectanceToReflectanceFilterType::Pointer filterSRToR = SurfaceReflectanceToReflectanceFilterType::New();

  filterSRToR->SetAtmosphericRadiativeTerms(m_AtmosphericRadiativeTerms);

  typename ImageType::Pointer image = ImageType::New();
  this->m_InputSpectralResponse->GetImage(image);
  filterSRToR->SetInput(image);
  filterSRToR->Update();

  this->m_CorrectedSpectralResponse->SetFromImage(filterSRToR->GetOutput());
  /*}
  else
    itkExceptionMacro(<< "Atmospheric radiative terms must be provided before updating the CSR (Corrected Spectral Response)"); */
}
} // end namespace otb

#endif
