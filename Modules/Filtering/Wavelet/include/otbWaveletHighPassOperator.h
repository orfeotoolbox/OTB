/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWaveletHighPassOperator_h
#define otbWaveletHighPassOperator_h

#include "otbWaveletOperatorBase.h"

namespace otb
{

/**
 * \class WaveletHighPassOperator
 * \brief Specific implementation of the mother wavelet part of
 * wavelet transformator
 *
 * This class implements the generic construction of the high pass filter
 * in a wavelet transformation.
 *
 * \sa WaveletOperator
 * \sa WaveletGenerator
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator, Wavelet::WaveletDirection TDirectionOfTransformation, class TPixel, unsigned int VDimension,
          class TAllocator = itk::NeighborhoodAllocator<TPixel>>
class ITK_EXPORT WaveletHighPassOperator : public WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef WaveletHighPassOperator Self;
  typedef WaveletOperatorBase<TMotherWaveletOperator, TPixel, VDimension, TAllocator> Superclass;

  itkTypeMacro(WaveletHighPassOperator, WaveletOperatorBase);

  typedef typename Superclass::SizeType  SizeType;
  static const Wavelet::WaveletDirection DirectionOfTransformation = TDirectionOfTransformation;

protected:
  void PrintSelf(std::ostream& os, itk::Indent i) const override
  {
    os << i << "WaveletHighPassOperator {this=" << this << "}" << std::endl;
    Superclass::PrintSelf(os, i.GetNextIndent());
  }

  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType         PixelType;

  /**
   * Set operator coefficients.
   */
  CoefficientVector GenerateCoefficients() override
  {
    CoefficientVector coeff;
    if (DirectionOfTransformation == Wavelet::FORWARD)
    {
      this->m_WaveletGenerator->GetHighPassCoefficientVector(coeff);
    }
    if (DirectionOfTransformation == Wavelet::INVERSE)
    {
      this->m_WaveletGenerator->GetLowPassCoefficientVector(coeff);
      Superclass::GenerateInverseHighPassFilterFromLowPassFilter(coeff);
    }

    Superclass::UpSamplingCoefficients(coeff);
    return coeff;
  }
}; // end of class

} // end of namespace otb

#endif
