/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbSurfaceAdjacencyEffectCorrectionSchemeFilter_hxx
#define otbSurfaceAdjacencyEffectCorrectionSchemeFilter_hxx

#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "otbImage.h"
#include "otbSIXSTraits.h"
#include "otbMath.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::SurfaceAdjacencyEffectCorrectionSchemeFilter()
  : m_IsSetAtmosphericRadiativeTerms(false),
    m_IsSetAtmoCorrectionParameters(false),
    m_IsSetAcquiCorrectionParameters(false),
    m_WindowRadius(1),
    m_FunctorParametersHaveBeenComputed(false),
    m_PixelSpacingInKilometers(1.),
    m_ZenithalViewingAngle(361.)
{
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
  m_AtmoCorrectionParameters  = AtmoCorrectionParametersType::New();
  m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();
}

template <class TInputImage, class TOutputImage>
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  this->GenerateParameters();
}

template <class TInputImage, class TOutputImage>
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::Modified() const
{
  Superclass::Modified();
  m_FunctorParametersHaveBeenComputed = false;
}

template <class TInputImage, class TOutputImage>
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::UpdateAtmosphericRadiativeTerms()
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
  if (!m_IsSetAcquiCorrectionParameters) // Get info from image metadata
  {
    const auto & metadata = this->GetInput()->GetImageMetadata();

    m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();

    m_AcquiCorrectionParameters->SetSolarZenithalAngle(90. - metadata[MDNum::SunElevation]);
    m_AcquiCorrectionParameters->SetSolarAzimutalAngle(metadata[MDNum::SunAzimuth]);
    m_AcquiCorrectionParameters->SetViewingZenithalAngle(90. - metadata[MDNum::SatElevation]);
    m_AcquiCorrectionParameters->SetViewingAzimutalAngle(metadata[MDNum::SatAzimuth]);

    m_AcquiCorrectionParameters->SetDay(metadata[MDTime::AcquisitionDate].GetDay());
    m_AcquiCorrectionParameters->SetMonth(metadata[MDTime::AcquisitionDate].GetMonth());


    if (metadata.HasBandMetadata(MDL1D::SpectralSensitivity))
    {
      auto spectralSensitivity = AcquiCorrectionParametersType::InternalWavelengthSpectralBandVectorType::New();
      for (const auto & band : metadata.Bands)
      {
        const auto & spectralSensitivityLUT = band[MDL1D::SpectralSensitivity];
        const auto & axis = spectralSensitivityLUT.Axis[0];
        auto filterFunction = FilterFunctionValues::New();
        // LUT1D stores a double vector whereas FilterFunctionValues stores a float vector
        std::vector<float> vec(spectralSensitivityLUT.Array.begin(), spectralSensitivityLUT.Array.end());
        filterFunction->SetFilterFunctionValues(vec);
        filterFunction->SetMinSpectralValue(axis.Origin);
        filterFunction->SetMaxSpectralValue(axis.Origin + axis.Spacing * (axis.Size-1));
        filterFunction->SetUserStep(axis.Spacing);
        spectralSensitivity->PushBack(filterFunction);
      }

      m_AcquiCorrectionParameters->SetWavelengthSpectralBand(spectralSensitivity);
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

  m_AtmosphericRadiativeTerms = CorrectionParametersToRadiativeTermsType::Compute(m_AtmoCorrectionParameters, m_AcquiCorrectionParameters);
}

template <class TInputImage, class TOutputImage>
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::GenerateParameters()
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
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>::UpdateFunctors()
{
  // get pointers to the input and output
  typename InputImageType::Pointer  inputPtr  = const_cast<TInputImage*>(this->GetInput());
  typename OutputImageType::Pointer outputPtr = const_cast<TOutputImage*>(this->GetOutput());

  WeightingMatrixType radiusMatrix(2 * m_WindowRadius + 1, 2 * m_WindowRadius + 1);
  radiusMatrix.Fill(0.);

  double center = static_cast<double>(m_WindowRadius);

  for (unsigned int i = 0; i < m_WindowRadius + 1; ++i)
  {
    for (unsigned int j = 0; j < m_WindowRadius + 1; ++j)
    {
      double id            = static_cast<double>(i);
      double jd            = static_cast<double>(j);
      double currentRadius = m_PixelSpacingInKilometers * std::sqrt(std::pow(id - center, 2) + std::pow(jd - center, 2));
      radiusMatrix(i, j)                                           = currentRadius;
      radiusMatrix(2 * m_WindowRadius - i, j)                      = currentRadius;
      radiusMatrix(2 * m_WindowRadius - i, 2 * m_WindowRadius - j) = currentRadius;
      radiusMatrix(i, 2 * m_WindowRadius - j)                      = currentRadius;
    }
  }

  for (unsigned int band = 0; band < inputPtr->GetNumberOfComponentsPerPixel(); ++band)
  {
    double rayleigh = m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForRayleigh(band);
    double aerosol  = m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForAerosol(band);

    WeightingMatrixType currentWeightingMatrix(2 * m_WindowRadius + 1, 2 * m_WindowRadius + 1);
    currentWeightingMatrix.Fill(0.);

    for (unsigned int i = 0; i < 2 * m_WindowRadius + 1; ++i)
    {
      for (unsigned int j = 0; j < 2 * m_WindowRadius + 1; ++j)
      {
        double notUsed1, notUsed2;
        double factor = 1;
        double palt   = 1000.;
        SIXSTraits::ComputeEnvironmentalContribution(rayleigh, aerosol, radiusMatrix(i, j), palt, std::cos(m_ZenithalViewingAngle * CONST_PI_180), notUsed1,
                                                     notUsed2,
                                                     factor); // Call to 6S
        currentWeightingMatrix(i, j) = factor;
      }
    }
    m_WeightingValues.push_back(currentWeightingMatrix);
  }

  DoubleContainerType upwardTransmittanceRatio, diffuseRatio;

  for (unsigned int band = 0; band < inputPtr->GetNumberOfComponentsPerPixel(); ++band)
  {
    upwardTransmittanceRatio.push_back(m_AtmosphericRadiativeTerms->GetUpwardTransmittance(band) /
                                       m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
    diffuseRatio.push_back(m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittance(band) / m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
  }
  this->GetFunctor().SetUpwardTransmittanceRatio(upwardTransmittanceRatio);
  this->GetFunctor().SetDiffuseRatio(diffuseRatio);
  this->GetFunctor().SetWeightingValues(m_WeightingValues);
}
/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Radius : " << m_WindowRadius << std::endl;
  os << indent << "Pixel spacing in kilometers: " << m_PixelSpacingInKilometers << std::endl;
  os << indent << "Zenithal viewing angle in degree: " << m_AcquiCorrectionParameters->GetViewingZenithalAngle() << std::endl;
}

} // end namespace otb

#endif
