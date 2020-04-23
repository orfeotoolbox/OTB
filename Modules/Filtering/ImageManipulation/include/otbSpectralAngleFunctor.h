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
#include <vector>
#include <numeric>

namespace otb
{
namespace Functor
{

namespace SpectralAngleDetails
{

/** \fn
 * \brief This function computes spectral angle between a pixel and a reference, the norm of these inputs.
 * should also be given as parameter of this function.
 * */
template <class TInput, class TReference, class TOutput>
TOutput ComputeSpectralAngle(TInput const & input, typename TInput ::ValueType const & inputNorm, 
                              TReference const & reference, typename TReference::ValueType refNorm)
{
  auto minSize = std::min(input.Size(), reference.Size());
  double scalarProduct = std::inner_product(&input[0], &input[minSize], &reference[0],0. );
  auto normProd = inputNorm * refNorm;
  if ((normProd < 1.e-12) || (scalarProduct / normProd > 1))
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

  ~SpectralAngleFunctor() = default;

  // Binary operator
  inline TOutputValue operator()(TInput const & inPix) const
  {
    return SpectralAngleDetails::ComputeSpectralAngle<TInput, TInput, TOutputValue>(inPix, inPix.GetNorm(), m_ReferencePixel, m_RefNorm);
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

/** \class SpectralAngleMapperFunctor
 *  \brief This functor computes the spectral angle according to a vector of reference pixel.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TReference, class TOutput>
class SpectralAngleMapperFunctor
{
public:
  SpectralAngleMapperFunctor() = default;
  virtual ~SpectralAngleMapperFunctor() = default;
  
  // Binary operator
  inline TOutput operator()(const TInput& inPix) const
  {
    TOutput res(m_ReferencePixels.size());
    
    auto inputNorm = inPix.GetNorm();
    
    for (unsigned int i = 0; i< m_ReferencePixels.size(); i++)
    {
      res[i] = SpectralAngleDetails::ComputeSpectralAngle<TInput, TInput, typename TOutput::ValueType>
                                      (inPix, inputNorm, m_ReferencePixels[i], m_ReferenceNorm[i]);
    }

    return res;
  }

  size_t OutputSize(...) const
  {
    return m_ReferencePixels.size();
  }

  void SetReferencePixels(std::vector<TReference> ref)
  {
    m_ReferencePixels = std::move(ref);
    m_ReferenceNorm.clear();
    // Precompute the norm of reference pixels
    for (auto const & pixel : m_ReferencePixels)
    {
      m_ReferenceNorm.push_back(pixel.GetNorm());
    }
  }
  
  std::vector<TReference> const & GetReferencePixels() const
  {
    return m_ReferencePixels;
  }

private:
  std::vector<TReference> m_ReferencePixels;
  std::vector<double> m_ReferenceNorm;
};

} // end namespace functor
} // end namespace otb

#endif
