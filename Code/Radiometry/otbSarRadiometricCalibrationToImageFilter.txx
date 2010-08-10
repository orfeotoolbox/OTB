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
#ifndef __otbSarRadiometricCalibrationToImageFilter_txx
#define __otbSarRadiometricCalibrationToImageFilter_txx

#include "otbSarRadiometricCalibrationToImageFilter.h"

#include "otbSarImageMetadataInterface.h"
#include "otbSarImageMetadataInterfaceFactory.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage>
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SarRadiometricCalibrationToImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{

	SarImageMetadataInterface::Pointer imageMetadataInterface = SarImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());

	FunctionPointer function = FunctionType::New();

	function->SetScale(imageMetadataInterface->GetRadiometricCalibrationScale());

    ParametricFunctionPointer   noise;
    ParametricFunctionPointer   antennaPatternNewGain;
    ParametricFunctionPointer   antennaPatternOldGain;
    ParametricFunctionPointer   incidenceAngle;
    ParametricFunctionPointer   rangeSpreadLoss;

    noise = function->GetNoise();
    noise->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationNoise());
    noise->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationNoisePolynomialDegree());
    function->SetNoise(noise);

    antennaPatternNewGain = function->GetAntennaPatternNewGain();
    antennaPatternNewGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGain());
    antennaPatternNewGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree());
    function->SetAntennaPatternNewGain(antennaPatternNewGain);

    antennaPatternOldGain = function->GetAntennaPatternOldGain();
    antennaPatternOldGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGain());
    antennaPatternOldGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree());
    function->SetAntennaPatternNewGain(antennaPatternOldGain);

    incidenceAngle = function->GetIncidenceAngle();
    incidenceAngle->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle());
    incidenceAngle->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationIncidenceAnglePolynomialDegree());
    function->SetAntennaPatternNewGain(incidenceAngle);

    rangeSpreadLoss = function->GetRangeSpreadLoss();
    rangeSpreadLoss->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLoss());
    rangeSpreadLoss->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree());
    function->SetAntennaPatternNewGain(rangeSpreadLoss);

    this->SetFunction(function);

    Superclass::BeforeThreadedGenerateData();
}


} // end namespace otb

#endif
