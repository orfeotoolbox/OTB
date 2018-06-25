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

#ifndef otbReflectanceToSurfaceReflectanceImageFilter_hxx
#define otbReflectanceToSurfaceReflectanceImageFilter_hxx

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::ReflectanceToSurfaceReflectanceImageFilter() :
 m_IsSetAtmosphericRadiativeTerms(false),
 m_IsSetAtmoCorrectionParameters(false),
 m_IsSetAcquiCorrectionParameters(false),
 m_UseGenerateParameters(true)
 {
    m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
    m_AtmoCorrectionParameters  = AtmoCorrectionParametersType::New();
    m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();
 }

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
 {
  Superclass::BeforeThreadedGenerateData();
  if (m_UseGenerateParameters) this->GenerateParameters();
 }


template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::Modified() const
{
  Superclass::Modified();
  m_FunctorParametersHaveBeenComputed  = false;
}


template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::GenerateParameters()
 {
  if (!m_IsSetAtmosphericRadiativeTerms)
    {
    this->UpdateAtmosphericRadiativeTerms();
    m_IsSetAtmosphericRadiativeTerms = true;
    }

  if (!m_FunctorParametersHaveBeenComputed)
    {
    this->UpdateFunctors();
    m_FunctorParametersHaveBeenComputed = true;
    }
 }


template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
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
          

  // load filter function values
  bool SetFilterFunctionValuesFileName=false;
  if (m_AcquiCorrectionParameters->GetFilterFunctionValuesFileName() != "")
    {
      m_AcquiCorrectionParameters->LoadFilterFunctionValue();
      SetFilterFunctionValuesFileName=true;
    }


  MetaDataDictionaryType dict = this->GetInput()->GetMetaDataDictionary();
  OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);


  if (m_AtmoCorrectionParameters->GetAeronetFileName() != "")
    m_AtmoCorrectionParameters->UpdateAeronetData(imageMetadataInterface->GetYear(),
                                              imageMetadataInterface->GetHour(),
                                              imageMetadataInterface->GetMinute());


  // Acquisition parameters
  if (!m_IsSetAcquiCorrectionParameters) // Get info from image metadata interface
      {
          m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();

          m_AcquiCorrectionParameters->SetSolarZenithalAngle(90. - imageMetadataInterface->GetSunElevation());
          m_AcquiCorrectionParameters->SetSolarAzimutalAngle(imageMetadataInterface->GetSunAzimuth());
          m_AcquiCorrectionParameters->SetViewingZenithalAngle(90. - imageMetadataInterface->GetSatElevation());
          m_AcquiCorrectionParameters->SetViewingAzimutalAngle(imageMetadataInterface->GetSatAzimuth());

          m_AcquiCorrectionParameters->SetDay(imageMetadataInterface->GetDay());
          m_AcquiCorrectionParameters->SetMonth(imageMetadataInterface->GetMonth());


          if (!SetFilterFunctionValuesFileName)
          {
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

      }


  m_AtmosphericRadiativeTerms = CorrectionParametersToRadiativeTermsType::Compute(m_AtmoCorrectionParameters,m_AcquiCorrectionParameters);

 }

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::UpdateFunctors()
 {

  if (this->GetInput() == nullptr)
    {
    itkExceptionMacro(<< "Input must be set before updating the functors");
    }
  
  MetaDataDictionaryType dict = this->GetInput()->GetMetaDataDictionary();
  OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

  this->GetFunctorVector().clear();

  for (unsigned int i = 0; i < this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
    {
    double       coef;
    double       res;
    // Don't change the order of atmospheric parameters.
    //FIXME Need to check in 6S that
    //unsigned int wavelengthPosition = imageMetadataInterface->BandIndexToWavelengthPosition(i);
    unsigned int wavelengthPosition = i;

    coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission(wavelengthPosition)
                               * m_AtmosphericRadiativeTerms->GetDownwardTransmittance(wavelengthPosition)
                               * m_AtmosphericRadiativeTerms->GetUpwardTransmittance(wavelengthPosition));
    coef = 1. / coef;
    res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(wavelengthPosition);
    FunctorType functor;
    functor.SetCoefficient(coef);
    functor.SetResidu(res);
    functor.SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo(wavelengthPosition)));

    this->GetFunctorVector().push_back(functor);

    otbMsgDevMacro(<< "Parameters to compute surface reflectance: ");

    otbMsgDevMacro(<< "Band wavelengh position:         " << wavelengthPosition);
    otbMsgDevMacro(<< "Coef (A):                        " << functor.GetCoefficient());
    otbMsgDevMacro(<< "Residu:                          " << functor.GetResidu());
    otbMsgDevMacro(<< "Spherical albedo:                " << functor.GetSphericalAlbedo());
    }
 }


/* Standard "PrintSelf" method */
template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Atmospheric radiative terms : " << m_AtmosphericRadiativeTerms << std::endl;
  os << indent << "Atmospheric correction terms : " << m_AtmoCorrectionParameters << std::endl;
  os << indent << "Acquisition correction terms : " << m_AcquiCorrectionParameters << std::endl;
}

} //end namespace otb

#endif
