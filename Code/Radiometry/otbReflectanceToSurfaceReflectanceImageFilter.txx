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
#ifndef __otbReflectanceToSurfaceReflectanceImageFilter_txx
#define __otbReflectanceToSurfaceReflectanceImageFilter_txx

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"
#include "otbOpticalImageMetadataInterface.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::ReflectanceToSurfaceReflectanceImageFilter() :
  m_IsSetAtmosphericRadiativeTerms(false),
  m_AeronetFileName(""),
  m_FilterFunctionValuesFileName(""),
  m_UseGenerateParameters(true)
{
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTerms::New();
  m_CorrectionParameters      = AtmosphericCorrectionParameters::New();
  m_FilterFunctionCoef.clear();
}

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::UpdateAtmosphericRadiativeTerms()
{
  if (this->GetInput() == NULL)
    {
    itkExceptionMacro(<< "Input must be set before updating the atmospheric radiative terms");
    }
  MetaDataDictionaryType dict = this->GetInput()->GetMetaDataDictionary();

  OpticalImageMetadataInterface::Pointer imageMetadataInterface = OpticalImageMetadataInterfaceFactory::CreateIMI(dict);

  if ((m_CorrectionParameters->GetDay() == 0))
    {
    m_CorrectionParameters->SetDay(imageMetadataInterface->GetDay());
    }

  if ((m_CorrectionParameters->GetMonth() == 0))
    {
    m_CorrectionParameters->SetMonth(imageMetadataInterface->GetMonth());
    }

  if ((m_CorrectionParameters->GetSolarZenithalAngle() == 361.))
    {
    m_CorrectionParameters->SetSolarZenithalAngle(90. - imageMetadataInterface->GetSunElevation());
    }

  if ((m_CorrectionParameters->GetSolarAzimutalAngle() == 361.))
    {
    m_CorrectionParameters->SetSolarAzimutalAngle(imageMetadataInterface->GetSunAzimuth());
    }

  if ((m_CorrectionParameters->GetViewingZenithalAngle() == 361.))
    {
    m_CorrectionParameters->SetViewingZenithalAngle(90. - imageMetadataInterface->GetSatElevation());
    }

  if ((m_CorrectionParameters->GetViewingAzimutalAngle() == 361.))
    {
    m_CorrectionParameters->SetViewingAzimutalAngle(imageMetadataInterface->GetSatAzimuth());
    }

  if (m_AeronetFileName != "")
    m_CorrectionParameters->UpdateAeronetData(m_AeronetFileName,
                                              imageMetadataInterface->GetYear(),
                                              imageMetadataInterface->GetHour(),
                                              imageMetadataInterface->GetMinute());

  // load filter function values
  if (m_FilterFunctionValuesFileName != "")
    {
    m_CorrectionParameters->LoadFilterFunctionValue(m_FilterFunctionValuesFileName);
    }
  // the user has set the filter function values
  else if (m_CorrectionParameters->GetWavelengthSpectralBand()->Size() !=
           this->GetInput()->GetNumberOfComponentsPerPixel())
    {
    bool ffvfOK = true;
    if (m_FilterFunctionCoef.size() == 0) ffvfOK = false;
    else if (m_FilterFunctionCoef.size() != this->GetInput()->GetNumberOfComponentsPerPixel())
      itkExceptionMacro(
        << "Function Values and Input image size mismatch");

    for (unsigned int i = 0; i < this->GetInput()->GetNumberOfComponentsPerPixel(); i++)
      {
      FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();

      if (ffvfOK)
        {
        functionValues->SetFilterFunctionValues(m_FilterFunctionCoef[i]);
        }
      else    // if no ffvf set, compute the step to be sure that the valueswavelength are between min and max and 1 as coef
        {
        functionValues->SetMinSpectralValue(imageMetadataInterface->GetFirstWavelengths()[i]);
        functionValues->SetMaxSpectralValue(imageMetadataInterface->GetLastWavelengths()[i]);
        //By default the function values size is 3, set the step to feet with min and max spectral value
        functionValues->SetUserStep((functionValues->GetMaxSpectralValue() - functionValues->GetMinSpectralValue()) / 2.);
        std::cout << "functionValues->GetMaxSpectralValue(): " << functionValues->GetMaxSpectralValue() << std::endl;
        std::cout << "functionValues->GetMinSpectralValue(): " << functionValues->GetMinSpectralValue() << std::endl;
        std::cout << "functionValues->GetUserStep(): " << functionValues->GetUserStep() << std::endl;
        std::cout << "functionValues size: " << functionValues->GetFilterFunctionValues().size() << std::endl;
        }

      m_CorrectionParameters->SetWavelengthSpectralBandWithIndex(i, functionValues);
      }

    }

  Parameters2RadiativeTermsPointerType param2Terms = Parameters2RadiativeTermsType::New();
  param2Terms->SetInput(m_CorrectionParameters);
  param2Terms->Update();
  m_AtmosphericRadiativeTerms = param2Terms->GetOutput();
}

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (m_UseGenerateParameters) this->GenerateParameters();
}

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::GenerateAtmosphericRadiativeTerms()
{
    Parameters2RadiativeTermsPointerType param2Terms = Parameters2RadiativeTermsType::New();
    param2Terms->SetInput(m_CorrectionParameters);
    //std::cout << " in GenerateAtmosphericRadiativeTerms before update vector values size before sixtraits " << m_CorrectionParameters->GetWavelengthSpectralBand()->GetNthElement(0)->GetFilterFunctionValues().size() << std::endl;
    param2Terms->Update();
    m_AtmosphericRadiativeTerms = param2Terms->GetOutput();
}

template <class TInputImage, class TOutputImage>
void
ReflectanceToSurfaceReflectanceImageFilter<TInputImage, TOutputImage>
::UpdateFunctors()
{
  if (this->GetInput() == NULL)
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
    unsigned int wavelengthPosition = imageMetadataInterface->BandIndexToWavelengthPosition(i);
    coef = static_cast<double>(m_AtmosphericRadiativeTerms->GetTotalGaseousTransmission(wavelengthPosition)
                               * m_AtmosphericRadiativeTerms->GetDownwardTransmittance(wavelengthPosition)
                               * m_AtmosphericRadiativeTerms->GetUpwardTransmittance(wavelengthPosition));
    coef = 1. / coef;
    res = -m_AtmosphericRadiativeTerms->GetIntrinsicAtmosphericReflectance(wavelengthPosition) * coef;
    FunctorType functor;
    functor.SetCoefficient(coef);
    functor.SetResidu(res);
    functor.SetSphericalAlbedo(static_cast<double>(m_AtmosphericRadiativeTerms->GetSphericalAlbedo(wavelengthPosition)));

    this->GetFunctorVector().push_back(functor);
    }
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

  this->UpdateFunctors();
}

}

#endif
