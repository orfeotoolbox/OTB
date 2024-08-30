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


#ifndef otbSarBrightnessFunction_hxx
#define otbSarBrightnessFunction_hxx

#include "otbSarBrightnessFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
SarBrightnessFunction<TInputImage, TCoordRep>::SarBrightnessFunction() : m_Scale(1.0)
{
  m_Noise                 = ParametricFunctionType::New();
  m_AntennaPatternNewGain = ParametricFunctionType::New();
  m_AntennaPatternOldGain = ParametricFunctionType::New();
  m_RangeSpreadLoss       = ParametricFunctionType::New();

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
void SarBrightnessFunction<TInputImage, TCoordRep>::SetInputImage(const InputImageType* ptr)
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
void SarBrightnessFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  if (m_Noise)
  {
    os << indent << "Noise:\n";
    m_Noise->Print(os, indent.GetNextIndent());
  }
  if (m_AntennaPatternNewGain)
  {
    os << indent << "AntennaPatternNewGain:\n";
    m_AntennaPatternNewGain->Print(os, indent.GetNextIndent());
  }
  if (m_AntennaPatternOldGain)
  {
    os << indent << "AntennaPatternOldGain:\n";
    m_AntennaPatternOldGain->Print(os, indent.GetNextIndent());
  }
  if (m_RangeSpreadLoss)
  {
    os << indent << "RangeSpreadLoss:\n";
    m_RangeSpreadLoss->Print(os, indent.GetNextIndent());
  }
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename SarBrightnessFunction<TInputImage, TCoordRep>::OutputType SarBrightnessFunction<TInputImage, TCoordRep>::EvaluateAtIndex(const IndexType& index) const
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


  FunctorRealType noise                 = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType antennaPatternNewGain = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType antennaPatternOldGain = itk::NumericTraits<FunctorRealType>::Zero;
  FunctorRealType rangeSpreadLoss       = itk::NumericTraits<FunctorRealType>::Zero;

  if (m_EnableNoise)
  {
    noise = static_cast<FunctorRealType>(m_Noise->EvaluateAtIndex(index));
  }
  antennaPatternNewGain = static_cast<FunctorRealType>(m_AntennaPatternNewGain->EvaluateAtIndex(index));
  antennaPatternOldGain = static_cast<FunctorRealType>(m_AntennaPatternOldGain->EvaluateAtIndex(index));
  rangeSpreadLoss       = static_cast<FunctorRealType>(m_RangeSpreadLoss->EvaluateAtIndex(index));

  FunctorType functor;
  if (m_EnableNoise)
  {
    functor.SetNoise(noise);
  }
  functor.SetScale(m_Scale);
  functor.SetAntennaPatternNewGain(antennaPatternNewGain);
  functor.SetAntennaPatternOldGain(antennaPatternOldGain);
  functor.SetRangeSpreadLoss(rangeSpreadLoss);

  const std::complex<float> pVal  = this->GetInputImage()->GetPixel(index);
  const RealType            value = std::sqrt((pVal.real() * pVal.real()) + (pVal.imag() * pVal.imag()));

  result = functor(value);

  return static_cast<OutputType>(result);
}

} // end namespace otb

#endif
