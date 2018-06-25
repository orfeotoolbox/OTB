/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSurfaceReflectanceToReflectanceFilter_hxx
#define otbSurfaceReflectanceToReflectanceFilter_hxx

#include "otbSurfaceReflectanceToReflectanceFilter.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::SurfaceReflectanceToReflectanceFilter() :
 m_IsSetAtmosphericRadiativeTerms(false),
 m_IsSetAtmoCorrectionParameters(false),
 m_IsSetAcquiCorrectionParameters(false),
 m_UseGenerateParameters(true)
{
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
  m_AtmoCorrectionParameters = AtmoCorrectionParametersType::New();
  m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::UpdateAtmosphericRadiativeTerms()
{
  if (this->GetInput() == nullptr)
    {
      itkExceptionMacro(<< "Input must be set before updating the atmospheric radiative terms");
    }

  // Atmospheric parameters
  if (!m_IsSetAtmoCorrectionParameters)
        {
          itkExceptionMacro(<< "Atmospheric correction parameters must be provided before updating the atmospheric radiative terms");
        }


  // Acquisition parameters
  if (!m_IsSetAcquiCorrectionParameters) // Get info from image metadata interface
      {
          MetaDataDictionaryType dict = this->GetInput()->GetMetaDataDictionary();
          OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

          m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();

          m_AcquiCorrectionParameters->SetSolarZenithalAngle(90. - imageMetadataInterface->GetSunElevation());
          m_AcquiCorrectionParameters->SetSolarAzimutalAngle(imageMetadataInterface->GetSunAzimuth());
          m_AcquiCorrectionParameters->SetViewingZenithalAngle(90. - imageMetadataInterface->GetSatElevation());
          m_AcquiCorrectionParameters->SetViewingAzimutalAngle(imageMetadataInterface->GetSatAzimuth());

          m_AcquiCorrectionParameters->SetDay(imageMetadataInterface->GetDay());
          m_AcquiCorrectionParameters->SetMonth(imageMetadataInterface->GetMonth());

          if (imageMetadataInterface->GetSpectralSensitivity()->Capacity() > 0)
                {
                    m_AcquiCorrectionParameters->SetWavelengthSpectralBand(imageMetadataInterface->GetSpectralSensitivity());
                    
                }
          else
                {
                    otbMsgDevMacro(<< "use dummy filter");
                    WavelengthSpectralBandVectorType spectralDummy;
                    spectralDummy->Clear();
                    for (unsigned int i = 0; i < this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
                      {
                        spectralDummy->PushBack(FilterFunctionValuesType::New());
                      }
                    m_AcquiCorrectionParameters->SetWavelengthSpectralBand(spectralDummy);
                }

      }

  m_AtmosphericRadiativeTerms = CorrectionParametersToRadiativeTermsType::Compute(m_AtmoCorrectionParameters,m_AcquiCorrectionParameters);
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  if(m_UseGenerateParameters)
    this->GenerateParameters();
}

template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::Modified() const
{
  Superclass::Modified();
  m_FunctorParametersHaveBeenComputed  = false;
}

template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::UpdateFunctors()
{
  //this->GetFunctor().clear();

//    for (unsigned int i = 0; i<this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
   for (unsigned int i = 0; i<m_AtmosphericRadiativeTerms->GetWavelengthSpectralBand().size(); ++i)
   {
      double coef;
      double res;
      coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission(i)
                                 * m_AtmosphericRadiativeTerms->GetDownwardTransmittance(i)
                                 * m_AtmosphericRadiativeTerms->GetUpwardTransmittance(i)     );
      //coef = 1. / coef;
//       res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(i) * coef;
      res = m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(i);
      FunctorType functor;
      functor.SetCoefficient(coef);
      functor.SetResidu(res);
      functor.SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo(i)));

      this->GetFunctorVector().push_back(functor);
//       this->SetFunctor(functor);
   }
}


template <class TInputImage, class TOutputImage>
void
SurfaceReflectanceToReflectanceFilter<TInputImage, TOutputImage>
::GenerateParameters()
{

  if (!m_IsSetAtmosphericRadiativeTerms)
    {
    this->UpdateAtmosphericRadiativeTerms();
    m_IsSetAtmosphericRadiativeTerms=true;
    }

  if (!m_FunctorParametersHaveBeenComputed)
    {
    this->UpdateFunctors();
    m_FunctorParametersHaveBeenComputed = true;
    }
}

}

#endif
