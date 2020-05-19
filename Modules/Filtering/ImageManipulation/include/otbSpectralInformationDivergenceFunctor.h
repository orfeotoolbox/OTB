/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSpectralInformationDivergenceFunctor_h
#define otbSpectralInformationDivergenceFunctor_h

#include "otbMath.h"
#include <algorithm>
#include <vector>
#include <numeric>

namespace otb
{
namespace Functor
{

/** \class SpectralAngleFunctor
 *  \brief This functor computes the spectral information divergence according to a reference pixel.
 *
 *  Du, Yingzi & Chang, Chein-I & Ren, Hsuan & Chang, Chein-Chi & Jensen, James & D'Amico, Francis. (2004). "
      "New Hyperspectral Discrimination Measure for Spectral Characterization. Optical Engineering - OPT ENG. 43."
      " 1777-1786. 10.1117/1.1766301.
 * 
 * \ingroup OTBImageManipulation
 */
template <class TInput, class TReference, class TOutput>
class SpectralInformationDivergenceFunctor
{
public:
  SpectralInformationDivergenceFunctor() = default;
  virtual ~SpectralInformationDivergenceFunctor() = default;
  
  using OutputValueType = typename TOutput::ValueType;
  
  // Binary operator
  inline TOutput operator()(const TInput& input) const
  {
    TOutput res;
    res.SetSize(m_ReferenceProbabilities.size());
    
    auto inputProbability = ComputeProbabilityMassFunction(input);
    
    for (unsigned int i = 0; i< m_ReferenceProbabilities.size(); i++)
    {
      res[i] = ComputeSpectralInformationDivergence(inputProbability, m_ReferenceProbabilities[i]);
    }

    return res;
  }

  size_t OutputSize(...) const
  {
    return m_ReferenceProbabilities.size();
  }

  void SetReferencePixels(std::vector<TReference> const & ref)
  {
    // We only store the probability mass function associated with the reference pixels, are the latter are not needed
    // in the sid computation.
    m_ReferenceProbabilities.clear();
    for (auto const & pixel : ref)
    {
      m_ReferenceProbabilities.push_back(ComputeProbabilityMassFunction(pixel));
    }
  }
  
private:
  inline TInput ComputeProbabilityMassFunction(TInput const & input) const
  {
    for (unsigned int i = 0; i < input.Size(); i++)
    {
      // Input pixel should be non negative (e.g. reflectance, radiance)
      if (input[i] <= 0) 
      {
        throw std::runtime_error("Input pixels of the spectral information divergence algorithm should be strictly positive.");
      }
    }
    
    return input / std::accumulate(&input[0], &input[input.Size()], 0.0);
  }

  inline OutputValueType ComputeSpectralInformationDivergence(TInput const & p, TInput const & q) const
  {
    assert(p.Size() == q.Size());
    OutputValueType sid = 0.0;
    for (unsigned int i = 0; i < p.Size(); i++)
    {
      // Compute SID : p[i] * std::log(p[i]/q[i]) + q[i] * std::log(q[i]/p[i]);
      sid += (p[i] - q[i]) * std::log(p[i]/q[i]);
    }
    return sid;
  }

  /** Probability mass function associated with the reference pixel */ 
  std::vector<TReference> m_ReferenceProbabilities;
};

} // end namespace functor
} // end namespace otb

#endif //otbSpectralInformationDivergenceFunctor_h
