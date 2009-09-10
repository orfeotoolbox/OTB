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
#ifndef __otbTerraSarIntraCalibrationImageFilter_txx
#define __otbTerraSarIntraCalibrationImageFilter_txx

#include "otbTerraSarIntraCalibrationImageFilter.h"
#include "otbImageMetadataInterfaceFactory.h"
#include "otbImageMetadataInterfaceBase.h"


namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
TerraSarIntraCalibrationImageFilter<TInputImage,TOutputImage>
::TerraSarIntraCalibrationImageFilter()
{
  this->SetRadius(0);
  m_CalFactor = 1.;
  m_NoiseRangeValidityMin = 0.;
  m_NoiseRangeValidityMax = 0.;
  m_NoiseRangeValidityRef = 0.;
  m_LocalIncidentAngle = 0.;
  m_NoisePolynomialCoefficientsList.clear();
  std::cout<<"TerraSarIntraCalibrationImageFilter"<<std::cout;
}

template <class TInputImage, class TOutputImage>
void
TerraSarIntraCalibrationImageFilter<TInputImage,TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  std::cout<<"BeforeThreadedGenerateData"<<std::cout;
  // Check that each polynomial noise have the same degree
  if(m_NoisePolynomialCoefficientsList.size() == 0)
    	itkExceptionMacro(<<"Empty NoisePolynomialCoefficientsList");
  for(unsigned int i=0; i<m_NoisePolynomialCoefficientsList.size()-1; i++)
    {
      if( m_NoisePolynomialCoefficientsList[i].size() != m_NoisePolynomialCoefficientsList[i+1].size() )
	itkExceptionMacro(<<"Wrong noise polynomial coefficient, degrees mismatch");
    }

  this->GetFunctor().SetCalFactor( m_CalFactor );
  this->GetFunctor().SetNoiseRangeValidityMin( m_NoiseRangeValidityMin );
  this->GetFunctor().SetNoiseRangeValidityMax( m_NoiseRangeValidityMax );
  this->GetFunctor().SetNoiseRangeValidityRef( m_NoiseRangeValidityRef );
  this->GetFunctor().SetLocalIncidentAngle( m_LocalIncidentAngle );
  this->GetFunctor().SetNoisePolynomialCoefficientsList( m_NoisePolynomialCoefficientsList );
  this->GetFunctor().SetImageSize( this->GetInput()->GetLargestPossibleRegion().GetSize() );
}



}

#endif
