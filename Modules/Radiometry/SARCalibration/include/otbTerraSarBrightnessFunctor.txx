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
#ifndef otbTerraSarBrightnessFunctor_txx
#define otbTerraSarBrightnessFunctor_txx

#include "otbTerraSarBrightnessFunctor.h"

#include "itkNumericTraits.h"

namespace otb
{
namespace Functor
{

template <class TInput, class TOutput>
TerraSarBrightnessFunctor<TInput, TOutput>
::TerraSarBrightnessFunctor()
{
  m_CalibrationFactor = itk::NumericTraits<double>::Zero;
  m_ResultsInDecibels = false;
  m_DefaultValue = 0.00001; // Default value is 10^-5
}

template <class TInput, class TOutput>
TOutput
TerraSarBrightnessFunctor<TInput, TOutput>
::operator() (const TInput &inPix)
  {
  // Formula: Beta^0 = Ks * |DN|^2

  // First, square the input pixel
  double squareInPix = vcl_pow(static_cast<double>(inPix), 2.);

  // Then apply the calibration factor
  double beta = m_CalibrationFactor * squareInPix;

  if (beta <= 0) beta = m_DefaultValue;

  // Results in decibels case
  if (m_ResultsInDecibels)
    {
    beta = 10 * vcl_log10(beta);
    }

  return static_cast<TOutput>(beta);
  }

template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarBrightnessFunctor<TInput, TOutput>
::operator() (const std::complex<TInput> &inPix)
  {
  // First, extract modulus and phase
  double modulus = vcl_sqrt(inPix.real() * inPix.real() + inPix.imag() * inPix.imag());
  double phase   = vcl_atan2(inPix.imag(), inPix.real());

  // Then, calibrate the modulus
  double beta = this->operator() (modulus);

  // Last, put back the phase
  std::complex<TOutput> out(std::polar(beta, phase));

  return out;
  }

} // namespace Functor

} // namespace otb
#endif
