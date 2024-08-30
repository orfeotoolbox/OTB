/*
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


#ifndef otbSarRadiometricCalibrationFunction_hxx
#define otbSarRadiometricCalibrationFunction_hxx

#include "otbSarRadiometricCalibrationFunction.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>::SarRadiometricCalibrationFunction()
  : m_Scale(1.0),
    m_EnableNoise(false),
    m_RescalingFactor(1.0),
    m_ApplyAntennaPatternGain(true),
    m_ApplyIncidenceAngleCorrection(true),
    m_ApplyRangeSpreadLossCorrection(true),
    m_ApplyLookupDataCorrection(false),
    m_ApplyRescalingFactor(false)
{
  /* initialize parametric functions */
  m_Noise                 = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_IncidenceAngle        = ParametricFunctionType::New();
  m_RangeSpreadLoss       = ParametricFunctionType::New();

  /* initialize default values in paramerticFunction instances  */
  m_Noise->SetConstantValue(0.0);
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_IncidenceAngle->SetConstantValue(CONST_PI_2);
  m_RangeSpreadLoss->SetConstantValue(1.0);
}

/**
 * Initialize by setting the input image
 */
template <class TInputImage, class TCoordRep>
void SarRadiometricCalibrationFunction<TInputImage, TCoordRep>::SetInputImage(const InputImageType* ptr)
{
  Superclass::SetInputImage(ptr);
  m_Noise->SetInputImage(ptr);
  m_IncidenceAngle->SetInputImage(ptr);
  m_AntennaPatternNewGain->SetInputImage(ptr);
  m_AntennaPatternOldGain->SetInputImage(ptr);
  m_RangeSpreadLoss->SetInputImage(ptr);
}

/**
 * Print
 */
template <class TInputImage, class TCoordRep>
void SarRadiometricCalibrationFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/* Function: EvaluateAtIndex. This computes the required values for each pixel
* whose index is given in indexType argument. To convert index to point it uses
* InputImage::TransformIndexToPhysicalPoint(). IncidenceAngle and similar are
* computed based on this calculated point in SarParametricFunction   */
template <class TInputImage, class TCoordRep>
typename SarRadiometricCalibrationFunction<TInputImage, TCoordRep>::OutputType
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>::EvaluateAtIndex(const IndexType& index) const
{

  if (!this->IsInsideBuffer(index))
  {
    itkDebugMacro(<< "ERROR with IsInsideBuffer");
    return (itk::NumericTraits<OutputType>::max());
  }

  /* convert index to point */
  PointType point;
  if (m_ApplyAntennaPatternGain || m_ApplyIncidenceAngleCorrection || m_ApplyRangeSpreadLossCorrection)
    this->GetInputImage()->TransformIndexToPhysicalPoint(index, point);

  /** digitalNumber:
    * For complex pixel type, std::abs() returns the modulus, which is
    * sqrt((I*I) + (Q*Q)). Where I and Q are real and imaginary part of the
    * complex pixel. So to get (I*I) + (Q*Q) in our calculation, the output
    * of std::abs() is squared. See below (digitalNumber * digitalNumber) where
    * digitalNumber is the output of std::abs() which is sqrt((I*I) + (Q*Q)). For
    * non-complex pixel types, std::abs() simply returns absolute value.
    */

  const std::complex<float> pVal          = this->GetInputImage()->GetPixel(index);
  const RealType            digitalNumber = std::sqrt((pVal.real() * pVal.real()) + (pVal.imag() * pVal.imag()));

  RealType sigma = m_Scale * digitalNumber * digitalNumber;

  /** subtract noise if enabled. */
  if (m_EnableNoise)
  {
    sigma -= static_cast<RealType>(m_Noise->Evaluate(point));
  }

  /** Apply incidence angle correction if needed */
  if (m_ApplyIncidenceAngleCorrection)
  {
    sigma *= std::sin(static_cast<RealType>(m_IncidenceAngle->Evaluate(point)));
  }

  /** Apply old and new antenna pattern gain. */
  if (m_ApplyAntennaPatternGain)
  {
    sigma *= static_cast<RealType>(m_AntennaPatternNewGain->Evaluate(point));
    sigma /= static_cast<RealType>(m_AntennaPatternOldGain->Evaluate(point));
  }

  /** Apply range spread loss if needed. */
  if (m_ApplyRangeSpreadLossCorrection)
  {
    sigma *= static_cast<RealType>(m_RangeSpreadLoss->Evaluate(point));
  }

  /** Lookup value has effect on for some sensors which does not required the
    * above values (incidence angle, rangespreadloss etc.. */
  if (m_ApplyLookupDataCorrection)
  {
    if (m_EnableNoise && m_NoiseLut)
    {
      sigma = std::max(0., sigma - m_NoiseLut->GetValue(index[0], index[1]));
    }

    RealType lutVal = static_cast<RealType>(m_Lut->GetValue(index[0], index[1]));

    sigma /= lutVal * lutVal;
  }

  /** rescaling factor has effect only with CosmoSkymed Products */
  if (m_ApplyRescalingFactor)
  {
    sigma /= m_RescalingFactor;
  }

  if (sigma < 0.0)
  {
    sigma = 0.0;
  }

  return static_cast<OutputType>(sigma);
}

} // end namespace otb

#endif
