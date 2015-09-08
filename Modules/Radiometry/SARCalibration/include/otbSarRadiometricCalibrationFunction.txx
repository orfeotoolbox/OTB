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

#ifndef __otbSarRadiometricCalibrationFunction_txx
#define __otbSarRadiometricCalibrationFunction_txx

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
, m_ApplyAntennaPatternGain(false)
, m_ApplyIncidenceAngleCorrection(false)
, m_ApplyRangeSpreadingLossCorrection(false)
, m_ApplyLookupDataCorrection(true)
, m_ApplyRescalingFactor(false)

{
  /* intialize parametric functions */
  m_Noise = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_IncidenceAngle = ParametricFunctionType::New();
  m_RangeSpreadLoss = ParametricFunctionType::New();

  /* intialize default values in paramerticFunction instances  */
  m_Noise->SetConstantValue(0.0);
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_IncidenceAngle->SetConstantValue(CONST_PI_2);
  m_RangeSpreadLoss->SetConstantValue(1.0);

  m_Lut = 0; //new LookupTableBase();

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

  if (!this->GetInputImage())
    {
    itkDebugMacro( <<"ERROR with GetInputImage()");
    return (itk::NumericTraits<OutputType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    itkDebugMacro( << "ERROR with IsInsideBuffer");
    return (itk::NumericTraits<OutputType>::max());
    }

  /* convert index to point */
  PointType point;
  if (m_ApplyAntennaPatternGain || m_ApplyIncidenceAngleCorrection || m_ApplyRangeSpreadingLossCorrection)
    this->GetInputImage()->TransformIndexToPhysicalPoint( index, point);

  /** instiantiate otbSarRadiometricCalibrationFunctor  */
  FunctorType functor;

  /** Apply noise if enabled. Default is 1.0 */
  if (m_EnableNoise)
    {
    functor.SetNoise(static_cast<FunctorRealType>(m_Noise->Evaluate(point)));
    }
  functor.SetScale(m_Scale);

    /** Apply antenna pattern gain if needed. Default is 1.0 */
  if (m_ApplyAntennaPatternGain)
    {
    functor.SetAntennaPatternNewGain(static_cast<FunctorRealType>(m_AntennaPatternNewGain->Evaluate(point)));
    functor.SetAntennaPatternOldGain(static_cast<FunctorRealType>(m_AntennaPatternOldGain->Evaluate(point)));
    }

    /** Apply incidence angle correction if needed. Default is 1.0 */
    if (m_ApplyIncidenceAngleCorrection)
    {
    functor.SetIncidenceAngle(static_cast<FunctorRealType>(m_IncidenceAngle->Evaluate(point)));
    }

    /** Apply range spread loss if needed. Default is 1.0 */
    if (m_ApplyRangeSpreadingLossCorrection)
    {
    functor.SetRangeSpreadLoss(static_cast<FunctorRealType>(m_RangeSpreadLoss->Evaluate(point)));
    }

    /** Apply lookup value if needed. Default is 1.0 */
    if (m_ApplyLookupDataCorrection)
    {
    FunctorRealType lutVal = m_Lut->GetValue(index[0], index[1]);
    functor.SetLutValue(lutVal * lutVal);
    }

    /** Apply rescaling factor if needed. Default is 1.0 */
    if (m_ApplyRescalingFactor)
    {
    functor.SetRescalingFactor(m_RescalingFactor);
    }

    /** Get pixel value.  The pixel is of type of std::complex and we use
      * vcl_abs to get real + imaginary as RealType */
    const RealType value = static_cast<RealType>(vcl_abs(this->GetInputImage()->GetPixel(index)));

  /** Do the computation via the functor's () operator*/
  RealType result = functor(value);

  return static_cast<OutputType>(result);
}

} // end namespace otb

#endif
