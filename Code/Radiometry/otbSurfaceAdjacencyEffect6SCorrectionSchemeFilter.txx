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

#ifndef __otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter_txx
#define __otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter_txx

#include "otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.h"

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
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::SurfaceAdjacencyEffect6SCorrectionSchemeFilter()
{
  m_WindowRadius = 1;
  m_ParametersHaveBeenComputed=false;
  m_PixelSpacingInKilometers = 1.;
  m_ZenithalViewingAngle = 361.;
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
  m_CorrectionParameters      = AtmosphericCorrectionParameters::New();
  m_IsSetAtmosphericRadiativeTerms = false;
  m_AeronetFileName = "";
  m_FilterFunctionValuesFileName = "";
  m_FilterFunctionCoef.clear();
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  this->GenerateParameters();

  if (!m_ParametersHaveBeenComputed)
    {
    this->ComputeParameters();
    m_ParametersHaveBeenComputed = true;
    }
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::Modified()
{
  Superclass::Modified();
  m_ParametersHaveBeenComputed  = false;
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::UpdateAtmosphericRadiativeTerms()
{
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

  if (m_ZenithalViewingAngle == 361.)
    {
    this->SetZenithalViewingAngle(90. - imageMetadataInterface->GetSatElevation());
    }

  if ((m_CorrectionParameters->GetViewingAzimutalAngle() == 361.))
    {
    m_CorrectionParameters->SetViewingAzimutalAngle(imageMetadataInterface->GetSatAzimuth());
    }

  if (m_AeronetFileName != "")
    {
    m_CorrectionParameters->UpdateAeronetData(m_AeronetFileName,
                                              imageMetadataInterface->GetYear(),
                                              imageMetadataInterface->GetHour(),
                                              imageMetadataInterface->GetMinute());
    }

  // load filter function values
  if (m_FilterFunctionValuesFileName != "")
    {
    m_CorrectionParameters->LoadFilterFunctionValue(m_FilterFunctionValuesFileName);
    }
  // the user has set the filter function values
  else
    {
    if (m_FilterFunctionCoef.size() != this->GetInput()->GetNumberOfComponentsPerPixel())
      {
      itkExceptionMacro(<< "Filter Function and image channels mismatch.");
      }
    for (unsigned int i = 0; i < this->GetInput()->GetNumberOfComponentsPerPixel(); ++i)
      {
      FilterFunctionValuesType::Pointer functionValues = FilterFunctionValuesType::New();
      functionValues->SetFilterFunctionValues(m_FilterFunctionCoef[i]);
      functionValues->SetMinSpectralValue(imageMetadataInterface->GetFirstWavelengths()[i]);
      functionValues->SetMaxSpectralValue(imageMetadataInterface->GetLastWavelengths()[i]);

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
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::GenerateParameters()
{
  if (m_IsSetAtmosphericRadiativeTerms == false)
    {
    this->UpdateAtmosphericRadiativeTerms();
    }
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::ComputeParameters()
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
SurfaceAdjacencyEffect6SCorrectionSchemeFilter<TInputImage, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Radius : " << m_WindowRadius << std::endl;
  os << indent << "Pixel spacing in kilometers: " << m_PixelSpacingInKilometers << std::endl;
  os << indent << "Zenithal viewing angle in degree: " << m_ZenithalViewingAngle << std::endl;
}

} // end namespace otb

#endif
