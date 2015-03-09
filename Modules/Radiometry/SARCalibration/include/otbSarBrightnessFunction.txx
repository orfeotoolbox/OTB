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

#ifndef __otbSarBrightnessFunction_txx
#define __otbSarBrightnessFunction_txx

#include "otbSarBrightnessFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarBrightnessFunction<TInputImage, TCoordRep>
::SarBrightnessFunction():
  m_Scale(1.0)
{
  m_Noise = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_RangeSpreadLoss = ParametricFunctionType::New();

  m_Noise->SetConstantValue(0.0);
  m_EnableNoise = true;
  m_AntennaPatternNewGain->SetConstantValue(1.0);
  m_AntennaPatternOldGain->SetConstantValue(1.0);
  m_RangeSpreadLoss->SetConstantValue(1.0);
}

/**
 * Initialize by setting the input image
 */
template <class TInputImage, class TCoordRep>
void
SarBrightnessFunction<TInputImage, TCoordRep>
::SetInputImage(
  const InputImageType * ptr )
{
  Superclass::SetInputImage(ptr);
  m_Noise->SetInputImage(ptr);
  m_AntennaPatternNewGain->SetInputImage(ptr);
  m_AntennaPatternOldGain->SetInputImage(ptr);
  m_RangeSpreadLoss->SetInputImage(ptr);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
SarBrightnessFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarBrightnessFunction<TInputImage, TCoordRep>
::OutputType
SarBrightnessFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  RealType result;
  result = itk::NumericTraits<RealType>::Zero;

  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<OutputType>::max());
    }

  if (!this->IsInsideBuffer(index))
    {
    return (itk::NumericTraits<OutputType>::max());
    }


  FunctorRealType noise = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType antennaPatternNewGain = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType antennaPatternOldGain = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType rangeSpreadLoss = itk::NumericTraits<FunctorRealType>::Zero;

  if (m_EnableNoise)
    {
    noise = static_cast<FunctorRealType>(m_Noise->EvaluateAtIndex(index));
    }
  antennaPatternNewGain = static_cast<FunctorRealType>(m_AntennaPatternNewGain->EvaluateAtIndex(index));
  antennaPatternOldGain = static_cast<FunctorRealType>(m_AntennaPatternOldGain->EvaluateAtIndex(index));
  rangeSpreadLoss = static_cast<FunctorRealType>(m_RangeSpreadLoss->EvaluateAtIndex(index));

  FunctorType functor;
  if (m_EnableNoise)
    {
    functor.SetNoise(noise);
    }
  functor.SetScale(m_Scale);
  functor.SetAntennaPatternNewGain(antennaPatternNewGain);
  functor.SetAntennaPatternOldGain(antennaPatternOldGain);
  functor.SetRangeSpreadLoss(rangeSpreadLoss);

  const RealType value = static_cast<RealType>(vcl_abs(this->GetInputImage()->GetPixel(index)));
  result = functor(value);

  return static_cast<OutputType>(result);
}

} // end namespace otb

#endif
