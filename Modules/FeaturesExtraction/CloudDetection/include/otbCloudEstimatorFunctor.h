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

#ifndef otbCloudEstimatorFunctor_h
#define otbCloudEstimatorFunctor_h

#include "otbSpectralAngleFunctor.h"

namespace otb
{
namespace Functor
{
/** \class CloudEstimatorFunctor
 *  \brief Functor to help with the cloud detection
 *
 *  This functor first computes the spectral angle according to a reference pixel.
 *  Then multiplies the result by a Gaussian coefficient
 *  And reverse the pixel values.
 *
 * \ingroup OTBCloudDetection
 */

template <class TInput, class TOutputValue>
class CloudEstimatorFunctor
{
public:
  typedef SpectralAngleFunctor<TInput, TOutputValue> SpectralAngleFunctorType;

  CloudEstimatorFunctor()
  {
    m_ReferencePixel.SetSize(4);
    m_ReferencePixel.Fill(1);
    m_RefNorm  = 2.0;
    m_Variance = 1.0;
    m_Denom    = 1.0;
  }

  virtual ~CloudEstimatorFunctor()
  {
  }
  inline TOutputValue operator()(const TInput& inPix) const
  {

    TOutputValue lOut;
    double       lRes          = 0.0;
    double       lCurPixNorm   = 0.0;
    double       lGaussianCoef = 1.0;

    // Compute the Gaussian Coef
    for (unsigned int i = 0; i < std::min(inPix.Size(), m_ReferencePixel.Size()); ++i)
    {
      lCurPixNorm += inPix[i] * inPix[i];
    }
    lCurPixNorm   = std::sqrt(static_cast<double>(lCurPixNorm));
    lGaussianCoef = std::exp(-std::pow((lCurPixNorm - m_RefNorm), 2) / m_Denom);

    // Reverse the SpectralAngle values and set them between [0; 1]
    lRes = lGaussianCoef * ((CONST_PI - m_SpectralAngleFunctor(inPix)) / CONST_PI);

    lOut = static_cast<TOutputValue>(lRes);
    return lOut;
  }

  void SetReferencePixel(TInput ref)
  {
    m_ReferencePixel = ref;
    m_SpectralAngleFunctor.SetReferencePixel(ref);
    m_RefNorm = 0.0;
    for (unsigned int i = 0; i < ref.Size(); ++i)
    {
      m_RefNorm += ref[i] * ref[i];
    }
    m_RefNorm = std::sqrt(static_cast<double>(m_RefNorm));
    SetVariance(m_Variance);
  }

  void SetVariance(double variance)
  {
    m_Variance = variance;
    m_Denom    = 2 * variance * variance * m_RefNorm * m_RefNorm;
  }

  TInput GetReferencePixel() const
  {
    return m_ReferencePixel;
  }
  double GetVariance() const
  {
    return m_Variance;
  }

protected:
  SpectralAngleFunctorType m_SpectralAngleFunctor;
  TInput                   m_ReferencePixel;
  double                   m_RefNorm;
  double                   m_Variance;
  double                   m_Denom;
};

} // end namespace functor
} // end namespace otb

#endif
