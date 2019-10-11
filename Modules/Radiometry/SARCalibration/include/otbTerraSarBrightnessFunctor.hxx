/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTerraSarBrightnessFunctor_hxx
#define otbTerraSarBrightnessFunctor_hxx

#include "otbTerraSarBrightnessFunctor.h"

#include "itkNumericTraits.h"

namespace otb
{
namespace Functor
{

template <class TInput, class TOutput>
TerraSarBrightnessFunctor<TInput, TOutput>::TerraSarBrightnessFunctor()
{
  m_CalibrationFactor = itk::NumericTraits<double>::Zero;
  m_ResultsInDecibels = false;
  m_DefaultValue      = 0.00001; // Default value is 10^-5
}

template <class TInput, class TOutput>
TOutput TerraSarBrightnessFunctor<TInput, TOutput>::operator()(const TInput& inPix)
{
  // Formula: Beta^0 = Ks * |DN|^2

  // First, square the input pixel
  double squareInPix = std::pow(static_cast<double>(inPix), 2.);

  // Then apply the calibration factor
  double beta = m_CalibrationFactor * squareInPix;

  if (beta <= 0)
    beta = m_DefaultValue;

  // Results in decibels case
  if (m_ResultsInDecibels)
  {
    beta = 10 * std::log10(beta);
  }

  return static_cast<TOutput>(beta);
}

template <class TInput, class TOutput>
std::complex<TOutput> TerraSarBrightnessFunctor<TInput, TOutput>::operator()(const std::complex<TInput>& inPix)
{
  // First, extract modulus and phase
  double modulus = std::sqrt(inPix.real() * inPix.real() + inPix.imag() * inPix.imag());
  double phase   = std::atan2(inPix.imag(), inPix.real());

  // Then, calibrate the modulus
  double beta = this->operator()(modulus);

  // Last, put back the phase
  std::complex<TOutput> out(std::polar(beta, phase));

  return out;
}

} // namespace Functor

} // namespace otb
#endif
