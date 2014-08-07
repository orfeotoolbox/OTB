/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSurfaceAdjacencyEffectCorrectionSchemeFilter_txx
#define __otbSurfaceAdjacencyEffectCorrectionSchemeFilter_txx

#include "otbSurfaceAdjacencyEffectCorrectionSchemeFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "otbImage.h"
#include "otbSIXSTraits.h"
#include "otbMath.h"
#include "otbOpticalImageMetadataInterfaceFactory.h"
#include "otbOpticalImageMetadataInterface.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
::SurfaceAdjacencyEffectCorrectionSchemeFilter() :
 m_IsSetAtmosphericRadiativeTerms(false),
 m_IsSetAtmoCorrectionParameters(false),
 m_IsSetAcquiCorrectionParameters(false),
 m_WindowRadius(1),
 m_FunctorParametersHaveBeenComputed(false),
 m_PixelSpacingInKilometers(1.),
 m_ZenithalViewingAngle(361.)
{
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
  m_AtmoCorrectionParameters = AtmoCorrectionParametersType::New();
  m_AcquiCorrectionParameters = AcquiCorrectionParametersType::New();
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  this->GenerateParameters();

}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
::Modified()
{
  Superclass::Modified();
  m_FunctorParametersHaveBeenComputed  = false;
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
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
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
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
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutputImage>
::UpdateFunctors()
{
  // get pointers to the input and output
  typename InputImageType::Pointer  inputPtr = const_cast<TInputImage *>(this->GetInput());
  typename OutputImageType::Pointer outputPtr = const_cast<TOutputImage *>(this->GetOutput());

  WeightingMatrixType radiusMatrix(2*m_WindowRadius + 1, 2*m_WindowRadius + 1);
  radiusMatrix.Fill(0.);

  double center = static_cast<double>(m_WindowRadius);

  for (unsigned int i = 0; i < m_WindowRadius + 1; ++i)
    {
    for (unsigned int j = 0; j < m_WindowRadius + 1; ++j)
      {
      double id = static_cast<double>(i);
      double jd = static_cast<double>(j);
      double currentRadius = m_PixelSpacingInKilometers * vcl_sqrt(vcl_pow(id - center, 2) + vcl_pow(jd - center, 2));
      radiusMatrix(i, j) = currentRadius;
      radiusMatrix(2 * m_WindowRadius - i, j) = currentRadius;
      radiusMatrix(2 * m_WindowRadius - i, 2 * m_WindowRadius - j) = currentRadius;
      radiusMatrix(i, 2 * m_WindowRadius - j) = currentRadius;
      }
    }

  for (unsigned int band = 0; band < inputPtr->GetNumberOfComponentsPerPixel(); ++band)
    {
    double rayleigh = m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForRayleigh(band);
    double aerosol =  m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForAerosol(band);
    
    WeightingMatrixType currentWeightingMatrix(2*m_WindowRadius + 1, 2*m_WindowRadius + 1);
    currentWeightingMatrix.Fill(0.);

    for (unsigned int i = 0; i < 2 * m_WindowRadius + 1; ++i)
      {
      for (unsigned int j = 0; j < 2 * m_WindowRadius + 1; ++j)
        {
        double notUsed1, notUsed2;
        double factor = 1;
        double palt = 1000.;
        SIXSTraits::ComputeEnvironmentalContribution(rayleigh,
                                                     aerosol,
                                                     radiusMatrix(i,j),
                                                     palt,
                                                     vcl_cos(m_ZenithalViewingAngle * CONST_PI_180),
                                                     notUsed1,
                                                     notUsed2,
                                                     factor);                                                                                                        //Call to 6S
        currentWeightingMatrix(i, j) = factor;
        }
      }
    m_WeightingValues.push_back(currentWeightingMatrix);
    }

  DoubleContainerType upwardTransmittanceRatio, diffuseRatio;

  for (unsigned int band = 0; band < inputPtr->GetNumberOfComponentsPerPixel(); ++band)
    {
    upwardTransmittanceRatio.push_back(m_AtmosphericRadiativeTerms->GetUpwardTransmittance(
                                         band) / m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
    diffuseRatio.push_back(m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittance(
                             band) / m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
    }
  this->GetFunctor().SetUpwardTransmittanceRatio(upwardTransmittanceRatio);
  this->GetFunctor().SetDiffuseRatio(diffuseRatio);
  this->GetFunctor().SetWeightingValues(m_WeightingValues);
}
/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
SurfaceAdjacencyEffectCorrectionSchemeFilter<TInputImage, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Radius : " << m_WindowRadius << std::endl;
  os << indent << "Pixel spacing in kilometers: " << m_PixelSpacingInKilometers << std::endl;
  os << indent << "Zenithal viewing angle in degree: " << m_AcquiCorrectionParameters->GetViewingZenithalAngle() << std::endl;
}

} // end namespace otb

#endif
