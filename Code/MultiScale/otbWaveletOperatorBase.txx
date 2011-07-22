/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletOperatorBase_txx
#define __otbWaveletOperatorBase_txx

#include "otbWaveletOperatorBase.h"

#include <cassert>

namespace otb {

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::PrintSelf(std::ostream& os, itk::Indent i) const
{
  os << i << "Wavelet kind : " << GetWaveletName() << "\n";
  os << i << "Up-Sampling factor " << GetUpSampleFactor() << "\n";
  Superclass::PrintSelf(os, i.GetNextIndent());
  os << i << "Wavelet coeff: [ ";
  for (typename Superclass::ConstIterator iter = Superclass::Begin();
       iter != Superclass::End();
       ++iter)
    {
    os << *iter << ' ';
    }
  os << "]\n";
}

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::UpSamplingCoefficients(CoefficientVector& coeff)
{
  if (m_UpSampleFactor <= 1) return;

  unsigned long radius = static_cast<unsigned long>(coeff.size()) >> 1;
  unsigned long upSampleRadius = radius * m_UpSampleFactor;

  CoefficientVector upSampledCoeff ( 2 * upSampleRadius + 1 );
  upSampledCoeff.assign(2 * upSampleRadius + 1, 0.);
  upSampledCoeff[upSampleRadius] = coeff[radius];

  for (unsigned int i = 1; i <= radius; ++i)
    {
    upSampledCoeff[upSampleRadius + m_UpSampleFactor * i] = coeff[radius + i];
    upSampledCoeff[upSampleRadius - m_UpSampleFactor * i] = coeff[radius - i];
    }
  coeff = upSampledCoeff;
}

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::RevertFilter(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();
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

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::GenerateInverseHighPassFilterFromLowPassFilter(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();

  CoefficientVector highPassCoeff(length + 2);

  double sign = -1;
  for (unsigned int i = 0; i < length; ++i)
    {
    highPassCoeff[i] = sign * coeff[i];
    sign *= -1.;
    }
  highPassCoeff[length] = 0.;
  highPassCoeff[length + 1] = 0.;

  coeff = highPassCoeff;

  // Note that the 0.0 value is obtained by affectation and not by
  // computation. It is fine to do an == comparison
  while ((coeff[0] == coeff[coeff.size() - 1]) && (coeff[0] == 0.0))
    {
    ReduceFilterLength(coeff);
    }
}

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::GenerateInverseLowPassFilterFromHighPassFilter(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();

  CoefficientVector highPassCoeff(length + 2);

  double sign = 1;
  for (unsigned int i = 0; i < length; ++i)
    {
    highPassCoeff[i] = sign * coeff[i];
    sign *= -1.;
    }
  highPassCoeff[length] = 0.;
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

template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension, class TAllocator>
void
WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
::ReduceFilterLength(CoefficientVector& coeff)
{
  const unsigned int length = coeff.size();
  assert(length >= 2);
  CoefficientVector newFilter(length - 2);
  for (unsigned int i = 0; i < newFilter.size(); ++i)
    {
    newFilter[i] = coeff[i + 1];
    }
  coeff = newFilter;
}

} // end of namespace otb

#endif
