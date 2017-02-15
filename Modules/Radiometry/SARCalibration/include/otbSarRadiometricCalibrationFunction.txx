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

#ifndef otbSarRadiometricCalibrationFunction_txx
#define otbSarRadiometricCalibrationFunction_txx

#include "otbSarRadiometricCalibrationFunction.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::SarRadiometricCalibrationFunction()
: m_Scale(1.0)
, m_EnableNoise(false)
, m_RescalingFactor(1.0)
, m_ApplyAntennaPatternGain(true)
, m_ApplyIncidenceAngleCorrection(true)
, m_ApplyRangeSpreadLossCorrection(true)
, m_ApplyLookupDataCorrection(false)
, m_ApplyRescalingFactor(false)

{
  /* initialize parametric functions */
  m_Noise = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_IncidenceAngle = ParametricFunctionType::New();
  m_RangeSpreadLoss = ParametricFunctionType::New();

  /* initialize default values in paramerticFunction instances  */
  m_Noise->SetConstantValue(0.0);
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_IncidenceAngle->SetConstantValue(CONST_PI_2);
  m_RangeSpreadLoss->SetConstantValue(1.0);

//  m_Lut = 0; //new LookupTableBase();

}

/**
 * Initialize by setting the input image
 */
template <class TInputImage, class TCoordRep>
void
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::SetInputImage(
  const InputImageType * ptr )
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
void
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/* Function: EvaluateAtIndex. This computes the required values for each pixel
* whose index is given in indexType argument. To convert index to point it uses
* InputImage::TransformIndexToPhysicalPoint(). IncidenceAngle and similar are
* computed based on this calculated point in SarParametricFunction   */
template <class TInputImage, class TCoordRep>
typename SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::OutputType
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{

  if (!this->IsInsideBuffer(index))
    {
    itkDebugMacro( << "ERROR with IsInsideBuffer");
    return (itk::NumericTraits<OutputType>::max());
    }

  /* convert index to point */
  PointType point;
  if (m_ApplyAntennaPatternGain || m_ApplyIncidenceAngleCorrection || m_ApplyRangeSpreadLossCorrection)
    this->GetInputImage()->TransformIndexToPhysicalPoint( index, point);

  /** digitalNumber:
    * For complex pixel type, vcl_abs() returns the modulus. which is
    * sqrt((I*I) + (Q*Q)). Where I and Q are real and imaginary part of the
    * complex pixel. So to to get (I*I) + (Q*Q) in our calculation, the output
    * of vcl_abs() is squared. See below (digitalNumber * digitalNumber) where
    * digitalNumber is the output of vcl_abs() which is sqrt((I*I) + (Q*Q)). For
    * non-complex pixel types, vcl_abs() simply returns absolute value.
    */

	const std::complex<float> pVal = this->GetInputImage()->GetPixel(index);
	const RealType digitalNumber = std::sqrt((pVal.real() * pVal.real()) + (pVal.imag()* pVal.imag()));

  RealType sigma = m_Scale * digitalNumber * digitalNumber;

  /** subtract noise if enabled. */
  if (m_EnableNoise)
    {
    sigma  -= static_cast<RealType>(m_Noise->Evaluate(point));
    }

  /** Apply incidence angle correction if needed */
  if (m_ApplyIncidenceAngleCorrection)
    {
    sigma *= vcl_sin(static_cast<RealType>(m_IncidenceAngle->Evaluate(point)));
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
    RealType lutVal = static_cast<RealType>(m_Lut->GetValue(index[0], index[1]));
    sigma /= lutVal * lutVal;
    }

  /** rescaling factor has effect only with CosmoSkymed Products */
  if (m_ApplyRescalingFactor)
    {
    sigma /= m_RescalingFactor;
    }

  if(sigma < 0.0)
    {
    sigma = 0.0;
    }

  return static_cast<OutputType>(sigma);
}

} // end namespace otb

#endif
