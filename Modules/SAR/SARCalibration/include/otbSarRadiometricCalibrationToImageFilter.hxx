/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSarRadiometricCalibrationToImageFilter_hxx
#define otbSarRadiometricCalibrationToImageFilter_hxx

#include "otbSarRadiometricCalibrationToImageFilter.h"
#include "otbSarCalibrationLookupData.h"
#include "otbSARMetadata.h"
#include <boost/any.hpp>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>::SarRadiometricCalibrationToImageFilter() : m_LookupSelected(0)
{
}

template <class TInputImage, class TOutputImage>
void SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Retrieving input/output pointers
  InputImagePointer inputPtr = this->GetInput();

  if (inputPtr.IsNull())
  {
    itkExceptionMacro(<< "At least one input is missing."
                      << " Input is missing :" << inputPtr.GetPointer())
  }

  OutputImagePointer outputPtr = this->GetOutput();
  if (outputPtr.IsNull())
  {
    itkExceptionMacro(<< "At least one output is missing."
                      << " Output is missing :" << outputPtr.GetPointer())
  }
}

template <class TInputImage, class TOutputImage>
void SarRadiometricCalibrationToImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  // will SetInputImage on the function
  Superclass::BeforeThreadedGenerateData();
  
  /** Retrieve the ImageMetadata */
  auto imd = this->GetInput()->GetImageMetadata();
  
  /** Fetch the SARCalib */
  std::unique_ptr<SARCalib> sarCalibPtr;
  if (imd.Has(MDGeom::SARCalib))
  {
    sarCalibPtr = std::make_unique<SARCalib>(boost::any_cast<SARCalib>(imd[MDGeom::SARCalib]));
  }
  else if ((imd.Bands.size() > 0) && imd.Bands[0].Has(MDGeom::SARCalib))
  {
    sarCalibPtr = std::make_unique<SARCalib>(boost::any_cast<SARCalib>(imd.Bands[0][MDGeom::SARCalib]));
  }
  else
      throw std::runtime_error("otbSarRadiometricCalibrationToImageFilter was not able to fetch the SARCalib metadata.");

  /** Get the SarRadiometricCalibrationFunction function instance.  */
  FunctionPointer function = this->GetFunction();

  /** Check if a calibration lookupdata is available with the
    * product. eg. Sentinel1. This means
    * A. The computation of the backscatter is based on this lookup value which
    * depends on the given product.*
    * B. The other value such as antenna pattern gain, rangespread loss, incidence
    * angle has no effect in calibration  */
  bool apply = sarCalibPtr->calibrationLookupFlag;

  /* Below lines will toggle the necessary flags which can help skip some
   * computation. For example, if there is lookup value and ofcourse antenna
   * pattern gain is not required. Even if we try to compute the value with
   * sarCalibetricFunction we  get 1. This is the safe side. But as we are so sure
   * we skip all those calls to EvaluateParametricCoefficient and also the
   * Evaluate(). For the function the value is 1 by default.
   */
  function->SetApplyAntennaPatternGain(!apply);
  function->SetApplyIncidenceAngleCorrection(!apply);
  function->SetApplyRangeSpreadLossCorrection(!apply);
  function->SetApplyRescalingFactor(!apply);
  function->SetApplyLookupDataCorrection(apply);

  if (imd.Has(MDNum::CalScale))
    function->SetScale(imd[MDNum::CalScale]);
  else if ((imd.Bands.size() > 0) && (imd.Bands[0].Has(MDNum::CalScale)))
    function->SetScale(imd.Bands[0][MDNum::CalScale]);

  /* Compute noise if enabled */
  if (function->GetEnableNoise())
  {
    // Use a denoising LUT if available (e.g Sentinel 1 thermal noise LUT)
    if (sarCalibPtr->calibrationLookupData.find(SarCalibrationLookupData::NOISE) 
          != sarCalibPtr->calibrationLookupData.end())
    {
      function->SetNoiseLookupData(sarCalibPtr->calibrationLookupData[SarCalibrationLookupData::NOISE]);
    }
    // Use a parametric function instead
    else
    {
      ParametricFunctionPointer noise;
      noise = function->GetNoise();
      noise->SetPointSet(sarCalibPtr->radiometricCalibrationNoise);
      noise->SetPolynomalSize(sarCalibPtr->radiometricCalibrationNoisePolynomialDegree);
      noise->EvaluateParametricCoefficient();
    }
  }

  /* Compute old and new antenna pattern gain */
  if (function->GetApplyAntennaPatternGain())
  {
    ParametricFunctionPointer antennaPatternNewGain;
    antennaPatternNewGain = function->GetAntennaPatternNewGain();
    antennaPatternNewGain->SetPointSet(sarCalibPtr->radiometricCalibrationAntennaPatternNewGain);
    antennaPatternNewGain->SetPolynomalSize(sarCalibPtr->radiometricCalibrationAntennaPatternNewGainPolynomialDegree);
    antennaPatternNewGain->EvaluateParametricCoefficient();

    ParametricFunctionPointer antennaPatternOldGain;
    antennaPatternOldGain = function->GetAntennaPatternOldGain();
    antennaPatternOldGain->SetPointSet(sarCalibPtr->radiometricCalibrationAntennaPatternOldGain);
    antennaPatternOldGain->SetPolynomalSize(sarCalibPtr->radiometricCalibrationAntennaPatternOldGainPolynomialDegree);
    antennaPatternOldGain->EvaluateParametricCoefficient();
  }

  /* Compute incidence angle */
  if (function->GetApplyIncidenceAngleCorrection())
  {
    ParametricFunctionPointer incidenceAngle;
    incidenceAngle = function->GetIncidenceAngle();
    incidenceAngle->SetPointSet(sarCalibPtr->radiometricCalibrationIncidenceAngle);
    incidenceAngle->SetPolynomalSize(sarCalibPtr->radiometricCalibrationIncidenceAnglePolynomialDegree);
    incidenceAngle->EvaluateParametricCoefficient();
  }

  /* Compute Range spread Loss */
  if (function->GetApplyRangeSpreadLossCorrection())
  {
    ParametricFunctionPointer rangeSpreadLoss;
    rangeSpreadLoss = function->GetRangeSpreadLoss();
    rangeSpreadLoss->SetPointSet(sarCalibPtr->radiometricCalibrationRangeSpreadLoss);
    rangeSpreadLoss->SetPolynomalSize(sarCalibPtr->radiometricCalibrationRangeSpreadLossPolynomialDegree);
    rangeSpreadLoss->EvaluateParametricCoefficient();
  }

  /** Get the lookupdata instance. Unlike all the above this is not a
   * parametricFunction instance. But rather an internal class in IMI called
   * SarCalibrationLookupData.
   *
   * NOTE: As the computation of lookup data for sensors is not universal. One must
   * provide a sub-class.
   * See Also: otbSentinel1ImageMetadataInterface, otbTerraSarImageMetadataInterface,
   * otbRadarsat2ImageMetadataInterface  
   */
  if (function->GetApplyLookupDataCorrection())
  {
    function->SetCalibrationLookupData(sarCalibPtr->calibrationLookupData[this->GetLookupSelected()]);
  }

  /** This was introduced for cosmoskymed which required a rescaling factor */
  if (function->GetApplyRescalingFactor())
  {
    function->SetRescalingFactor(sarCalibPtr->rescalingFactor);
  }
}

} // end namespace otb

#endif
