/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbWaveletOperatorBase_hxx
#define otbWaveletOperatorBase_hxx

#include "otbWaveletOperatorBase.h"

#include <cassert>

namespace otb
{

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::PrintSelf(std::ostream& os, itk::Indent i) const
{
  os << i << "Wavelet kind : " << GetWaveletName() << "\n";
  os << i << "Up-Sampling factor " << GetUpSampleFactor() << "\n";
  Superclass::PrintSelf(os, i.GetNextIndent());
  os << i << "Wavelet coeff: [ ";
  for (typename Superclass::ConstIterator iter = Superclass::Begin(); iter != Superclass::End(); ++iter)
  {
    os << *iter << ' ';
  }
  os << "]\n";
}

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::UpSamplingCoefficients(CoefficientVector& coeff)
{
  if (m_UpSampleFactor <= 1)
    return;

  unsigned long radius         = static_cast<unsigned long>(coeff.size()) >> 1;
  unsigned long upSampleRadius = radius * m_UpSampleFactor;

  CoefficientVector upSampledCoeff(2 * upSampleRadius + 1);
  upSampledCoeff.assign(2 * upSampleRadius + 1, 0.);
  upSampledCoeff[upSampleRadius] = coeff[radius];

  for (unsigned int i = 1; i <= radius; ++i)
  {
    upSampledCoeff[upSampleRadius + m_UpSampleFactor * i] = coeff[radius + i];
    upSampledCoeff[upSampleRadius - m_UpSampleFactor * i] = coeff[radius - i];
  }
  coeff = upSampledCoeff;
}

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::RevertFilter(CoefficientVector& coeff)
{
  const unsigned int length         = coeff.size();
  const unsigned int medianPosition = length / 2;

  CoefficientVector newCoeff(length);
  newCoeff[medianPosition] = coeff[medianPosition];
  for (unsigned int i = 1; i <= medianPosition; ++i)
  {
    newCoeff[medianPosition + i] = coeff[medianPosition - i];
    newCoeff[medianPosition - i] = coeff[medianPosition + i];
  }

  coeff = newCoeff;
}

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::GenerateInverseHighPassFilterFromLowPassFilter(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();

  CoefficientVector highPassCoeff(length + 2);

  double sign = -1;
  for (unsigned int i = 0; i < length; ++i)
  {
    highPassCoeff[i] = sign * coeff[i];
    sign *= -1.;
  }
  highPassCoeff[length]     = 0.;
  highPassCoeff[length + 1] = 0.;

  coeff = highPassCoeff;

  // Note that the 0.0 value is obtained by affectation and not by
  // computation. It is fine to do an == comparison
  while ((coeff[0] == coeff[coeff.size() - 1]) && (coeff[0] == 0.0))
  {
    ReduceFilterLength(coeff);
  }
}

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::GenerateInverseLowPassFilterFromHighPassFilter(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();

  CoefficientVector highPassCoeff(length + 2);

  double sign = 1;
  for (unsigned int i = 0; i < length; ++i)
  {
    highPassCoeff[i] = sign * coeff[i];
    sign *= -1.;
  }
  highPassCoeff[length]     = 0.;
  highPassCoeff[length + 1] = 0.;

  coeff = highPassCoeff;

  // Note that the 0.0 value is obtained by affectation and not by
  // computation. It is fine to do an == comparison
  while ((coeff[0] == coeff[coeff.size() - 1]) && (coeff[0] == 0.0))
  {
    assert(coeff.size() > 1);
    ReduceFilterLength(coeff);
  }
}

template <Wavelet::Wavelet TMotherWaveletOperator, class TPixel, unsigned int VDimension, class TAllocator>
void WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>::ReduceFilterLength(CoefficientVector& coeff)
{
  assert(coeff.size() >= 2);
  coeff.pop_back();
  coeff.erase(coeff.begin());
}

} // end of namespace otb

#endif
