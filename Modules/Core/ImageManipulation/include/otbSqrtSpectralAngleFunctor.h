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

#ifndef otbSqrtSpectralAngleFunctor_h
#define otbSqrtSpectralAngleFunctor_h

#include "otbSpectralAngleFunctor.h"
#include "itkMacro.h"

namespace otb
{

namespace Functor
{

/** \class SqrtSpectralAngleFunctor
 *  \brief This functor computes the square root of a spectral angle according to a reference pixel.
 *
 * \ingroup OTBImageManipulation
 */

/** \class SpectralAngleFunctor
 *  \brief This functor computes the spectral angle according to a reference pixel.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutputValue>
class SqrtSpectralAngleFunctor
{
public:
  SqrtSpectralAngleFunctor()
  {
    m_ReferencePixel.SetSize(4);
    m_ReferencePixel.Fill(1);
  }

  virtual ~SqrtSpectralAngleFunctor() = default;

  // Binary operator
  inline TOutputValue operator()(TInput const & inPix) const
  {
    return std::sqrt(SpectralAngleDetails::ComputeSpectralAngle<TInput, TInput, TOutputValue>(inPix, inPix.GetNorm(), m_ReferencePixel, m_RefNorm));
  }

  void SetReferencePixel(TInput const & ref)
  {
    m_ReferencePixel = ref;
    m_RefNorm = ref.GetNorm();
  }
  
  TInput GetReferencePixel() const
  {
    return m_ReferencePixel;
  }

private :
  TInput m_ReferencePixel;
  double m_RefNorm;
};

} // end namespace Functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
