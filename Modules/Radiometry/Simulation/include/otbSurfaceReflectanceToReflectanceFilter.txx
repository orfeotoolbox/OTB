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
#ifndef __otbSurfaceReflectanceToReflectanceFilter_txx
#define __otbSurfaceReflectanceToReflectanceFilter_txx

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
  if (this->GetInput() == NULL)
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
::Modified()
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
