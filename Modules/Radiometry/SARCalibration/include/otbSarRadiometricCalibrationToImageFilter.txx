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
#include "otbSarImageMetadataInterfaceFactory.h"
#include "otbSarCalibrationLookupData.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage>
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SarRadiometricCalibrationToImageFilter()
: m_Initialized(false)
{
  this->SetNumberOfThreads( 1 );
}

template<class TInputImage, class TOutputImage>
void
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::SetupCalibrator()
{

  SarImageMetadataInterface::Pointer imageMetadataInterface = SarImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());

  FunctionPointer function = this->GetFunction();

  bool apply = imageMetadataInterface->HasCalibrationLookupData();
  function->SetApplyAntennaPatternGain(!apply);
  function->SetApplyIncidenceAngleCorrection(!apply);
  function->SetApplyRangeSpreadingLossCorrection(!apply);
  function->SetApplyRescalingFactor(!apply);
  function->SetApplyLookupDataCorrection(apply);

  function->SetScale(imageMetadataInterface->GetRadiometricCalibrationScale());

  if( function->GetEnableNoise())
    {
    ParametricFunctionPointer   noise;
    noise = function->GetNoise();
    noise->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationNoise());
    noise->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationNoisePolynomialDegree());
    noise->EvaluateParametricCoefficient();
    }
  if(function->GetApplyAntennaPatternGain())
    {
    ParametricFunctionPointer   antennaPatternNewGain;
    antennaPatternNewGain = function->GetAntennaPatternNewGain();
    antennaPatternNewGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGain());
    antennaPatternNewGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree());
    antennaPatternNewGain->EvaluateParametricCoefficient();

    ParametricFunctionPointer   antennaPatternOldGain;
    antennaPatternOldGain = function->GetAntennaPatternOldGain();
    antennaPatternOldGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGain());
    antennaPatternOldGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree());
    antennaPatternOldGain->EvaluateParametricCoefficient();
    }

  if (function->GetApplyIncidenceAngleCorrection())
    {
    ParametricFunctionPointer   incidenceAngle;
    incidenceAngle = function->GetIncidenceAngle();
    incidenceAngle->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle());
    incidenceAngle->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationIncidenceAnglePolynomialDegree());
    incidenceAngle->EvaluateParametricCoefficient();
    }

  if (function->GetApplyRangeSpreadingLossCorrection())
    {
    ParametricFunctionPointer   rangeSpreadLoss;
    rangeSpreadLoss = function->GetRangeSpreadLoss();
    rangeSpreadLoss->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLoss());
    rangeSpreadLoss->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree());
    rangeSpreadLoss->EvaluateParametricCoefficient();
    }
  if (function->GetApplyLookupDataCorrection())
    {
    function->SetCalibrationLookupData(imageMetadataInterface->GetCalibrationLookupData("sigma"));
    }

  /*  http://www.e-geos.it/products/pdf/COSMO-SkyMed-Image_Calibration.pdf */
  if (function->GetApplyRescalingFactor())
    {
    function->SetRescalingFactor(imageMetadataInterface->GetRescalingFactor());
    }

  m_Initialized = true;

}

template<class TInputImage, class TOutputImage>
void
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();

  if( !m_Initialized)
    this->SetupCalibrator();

}

} // end namespace otb

#endif
