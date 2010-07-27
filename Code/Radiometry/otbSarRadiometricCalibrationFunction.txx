/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSarRadiometricCalibrationFunction_txx
#define __otbSarRadiometricCalibrationFunction_txx

#include "otbSarRadiometricCalibrationFunction.h"
#include "otbSarParametricMapFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::SarRadiometricCalibrationFunction():
  m_Offset(0.0),
  m_Scale(1.0),
  m_IncidenceAngle(ParametricFunctionType::New()),
  m_AntennaPatternNewGain(ParametricFunctionType::New()),
  m_AntennaPatternOldGain(ParametricFunctionType::New()),
  m_RangeSpreadLoss(ParametricFunctionType::New())
{
  m_IncidenceAngle->SetConstantValue(90.0);
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_RangeSpreadLoss->SetConstantValue(1.0);
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
    m_IncidenceAngle->SetInputImage(ptr);
    m_AntennaPatternNewGain->SetInputImage(ptr);
    m_AntennaPatternOldGain->SetInputImage(ptr);
    m_RangeSpreadLoss->SetInputImage(ptr);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::RealType
SarRadiometricCalibrationFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType result;

  result = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<RealType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    return (itk::NumericTraits<RealType>::max());
    }

  FunctorRealType AntennaPatternNewGain;
  FunctorRealType AntennaPatternOldGain;
  FunctorRealType IncidenceAngle; 
  FunctorRealType RangeSpreadLoss;

  AntennaPatternNewGain = m_AntennaPatternNewGain->EvaluateAtIndex(index);
  AntennaPatternOldGain = m_AntennaPatternOldGain->EvaluateAtIndex(index);
  IncidenceAngle = m_IncidenceAngle->EvaluateAtIndex(index);
  RangeSpreadLoss = m_RangeSpreadLoss->EvaluateAtIndex(index);
   
  FunctorType functor;
  functor.SetOffset(m_Offset);
  functor.SetScale(m_Scale);
  functor.SetAntennaPatternNewGain(AntennaPatternNewGain);
  functor.SetAntennaPatternOldGain(AntennaPatternOldGain);
  functor.SetIncidenceAngle(IncidenceAngle);
  functor.SetRangeSpreadLoss(RangeSpreadLoss);

  const RealType value = static_cast<RealType>(this->GetInputImage()->GetPixel(index));
  result = functor.operator ()( value);

  return result;
}

} // end namespace otb

#endif
