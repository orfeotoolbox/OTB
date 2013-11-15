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
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();

  SarImageMetadataInterface::Pointer imageMetadataInterface = SarImageMetadataInterfaceFactory::CreateIMI(
      this->GetInput()->GetMetaDataDictionary());

  FunctionPointer function = this->GetFunction();

  function->SetScale(imageMetadataInterface->GetRadiometricCalibrationScale());

  ParametricFunctionPointer   noise;
  ParametricFunctionPointer   antennaPatternNewGain;
  ParametricFunctionPointer   antennaPatternOldGain;
  ParametricFunctionPointer   incidenceAngle;
  ParametricFunctionPointer   rangeSpreadLoss;

  noise = function->GetNoise();
  noise->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationNoise());
  noise->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationNoisePolynomialDegree());
  noise->EvaluateParametricCoefficient();

  antennaPatternNewGain = function->GetAntennaPatternNewGain();
  antennaPatternNewGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGain());
  antennaPatternNewGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree());
  antennaPatternNewGain->EvaluateParametricCoefficient();

  antennaPatternOldGain = function->GetAntennaPatternOldGain();
  antennaPatternOldGain->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGain());
  antennaPatternOldGain->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree());
  antennaPatternOldGain->EvaluateParametricCoefficient();

  incidenceAngle = function->GetIncidenceAngle();
  incidenceAngle->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle());

  typename ParametricFunctionType::PointType point;
  point.Fill(0);
  typename ParametricFunctionType::PointSetType::PixelType pointValue;
  pointValue = itk::NumericTraits<typename ParametricFunctionType::PointSetType::PixelType>::Zero;
  unsigned int nbRecords = imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle()->GetNumberOfPoints();

  // Fill the linear system
  for (unsigned int i = 0; i < nbRecords; ++i)
  {
      imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle()->GetPoint(i, &point);
      imageMetadataInterface->GetRadiometricCalibrationIncidenceAngle()->GetPointData(i, &pointValue);
  }
  incidenceAngle->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationIncidenceAnglePolynomialDegree());

  incidenceAngle->EvaluateParametricCoefficient();
  rangeSpreadLoss = function->GetRangeSpreadLoss();
  rangeSpreadLoss->SetPointSet(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLoss());
  rangeSpreadLoss->SetPolynomalSize(imageMetadataInterface->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree());
  rangeSpreadLoss->EvaluateParametricCoefficient();

}


} // end namespace otb

#endif
