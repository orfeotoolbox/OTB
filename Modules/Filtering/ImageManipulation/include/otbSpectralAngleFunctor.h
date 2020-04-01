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

#ifndef otbSpectralAngleFunctor_h
#define otbSpectralAngleFunctor_h

#include "otbMath.h"
#include <algorithm>

namespace otb
{
namespace Functor
{

namespace SpectralAngleDetails
{

template <class TInput, class TReference, class TOutput>
TOutput ComputeSpectralAngle(TInput const & input, TReference const & reference, 
                              typename TReference::ValueType refNorm)
{
  // Compute norm and scalar product.
  double scalarProduct   = 0.0;
  double squaredNormInput  = 0.0;
  for (unsigned int i = 0; i < std::min(input.Size(), reference.Size()); ++i)
  {
    scalarProduct += input[i] * reference[i];
    squaredNormInput += input[i] * input[i];
  }
  auto normProd = std::sqrt(squaredNormInput) * refNorm;
  if ((normProd == 0.0) || (scalarProduct / normProd > 1))
  {
    return static_cast<TOutput>(0.0);
  }
  else
  {
    return static_cast<TOutput>(std::acos(scalarProduct / normProd));
  }
}

} // end namespace SpectralAngleDetails

/** \class SpectralAngleFunctor
 *  \brief This functor computes the spectral angle according to a reference pixel.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TOutputValue>
class SpectralAngleFunctor
{
public:
  SpectralAngleFunctor()
  {
    m_ReferencePixel.SetSize(4);
    m_ReferencePixel.Fill(1);
  }

  virtual ~SpectralAngleFunctor() = default;

  // Binary operator
  inline TOutputValue operator()(TInput const & inPix) const
  {
    return SpectralAngleDetails::ComputeSpectralAngle<TInput, TInput, TOutputValue>(inPix, m_ReferencePixel, m_RefNorm);
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


  
  

} // end namespace functor
} // end namespace otb

#endif
