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

#ifndef __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_txx
#define __otbSurfaceAdjencyEffect6SCorrectionSchemeFilter_txx

#include "otbSurfaceAdjencyEffect6SCorrectionSchemeFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "otbImage.h"
#include "otbSIXSTraits.h"
#include "otbAtmosphericRadiativeTerms.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::SurfaceAdjencyEffect6SCorrectionSchemeFilter()
{
  m_WindowRadius = 1;
  m_PixelSpacingInKilometers = 1.;
  m_ZenithalViewingAngle = 0;
  m_AtmosphericRadiativeTerms = AtmosphericRadiativeTermsType::New();
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  typename InputImageType::Pointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
  typename OutputImageType::Pointer outputPtr = const_cast< TOutputImage * >( this->GetOutput() );

  if(!inputPtr || !outputPtr)
    return;
  outputPtr->SetNumberOfComponentsPerPixel(inputPtr->GetNumberOfComponentsPerPixel());
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::Modified()
{
  Superclass::Modified();
  m_ParametersHaveBeenComputed  = false;
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData ()
{
  if(!m_ParametersHaveBeenComputed)
    {
      this->ComputeParameters();
      m_ParametersHaveBeenComputed = true;
    }
}

template <class TInputImage, class TOutputImage>
void
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutputImage>
::ComputeParameters()
{
  // get pointers to the input and output
  typename InputImageType::Pointer inputPtr = const_cast< TInputImage * >( this->GetInput() );
  typename OutputImageType::Pointer outputPtr = const_cast< TOutputImage * >( this->GetOutput() );


  WeightingMatrixType radiusMatrix(2*m_WindowRadius+1,2*m_WindowRadius+1);
  radiusMatrix.Fill(0.);

  double center = static_cast<double>(m_WindowRadius);

  for(unsigned int i = 0; i<m_WindowRadius+1; ++i)
    {
      for(unsigned int j = 0; j<m_WindowRadius+1; ++j)
  {
    double id = static_cast<double>(i);
    double jd = static_cast<double>(j);
    double currentRadius = m_PixelSpacingInKilometers*vcl_sqrt(vcl_pow(id-center,2)+vcl_pow(jd-center,2));
    radiusMatrix(i,j)=currentRadius;
    radiusMatrix(2*m_WindowRadius-i,j)=currentRadius;
    radiusMatrix(2*m_WindowRadius-i,2*m_WindowRadius-j)=currentRadius;
    radiusMatrix(i,2*m_WindowRadius-j)=currentRadius;
  }
    }

  for(unsigned int band = 0; band<inputPtr->GetNumberOfComponentsPerPixel();++band)
    {
      WeightingMatrixType currentWeightingMatrix(2*m_WindowRadius+1,2*m_WindowRadius+1);
      double rayleigh = m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForRayleigh(band);
      double aerosol =  m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittanceForAerosol(band);

      currentWeightingMatrix.Fill(0.);

      for(unsigned int i = 0; i<2*m_WindowRadius+1; ++i)
  {
    for(unsigned int j = 0; j<2*m_WindowRadius+1; ++j)
      {
        double notUsed1,notUsed2;
        double factor = 1;
        double palt = 1000.;
        SIXSTraits::ComputeEnvironmentalContribution(rayleigh,aerosol,radiusMatrix(i,j),palt,vcl_cos(m_ZenithalViewingAngle*M_PI/180.),notUsed1,notUsed2,factor); //Call to 6S
        currentWeightingMatrix(i,j)=factor;
      }
  }
      m_WeightingValues.push_back(currentWeightingMatrix);
    }


  DoubleContainerType upwardTransmittanceRatio,diffuseRatio;

  for(unsigned int band = 0; band<inputPtr->GetNumberOfComponentsPerPixel();++band)
    {
      upwardTransmittanceRatio.push_back(m_AtmosphericRadiativeTerms->GetUpwardTransmittance(band)/m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
      diffuseRatio .push_back(m_AtmosphericRadiativeTerms->GetUpwardDiffuseTransmittance(band)/m_AtmosphericRadiativeTerms->GetUpwardDirectTransmittance(band));
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
SurfaceAdjencyEffect6SCorrectionSchemeFilter<TInputImage, TOutput>
::PrintSelf( std::ostream& os, itk::Indent indent) const
{
  os << indent << "Radius : " << m_WindowRadius << std::endl;
  os << indent << "Pixel spacing in kilometers: "<<m_PixelSpacingInKilometers <<std::endl;
  os << indent << "Zenithal viewing angle in degree: "<<m_ZenithalViewingAngle<<std::endl;
}

} // end namespace otb

#endif
