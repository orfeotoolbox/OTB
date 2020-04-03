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

#ifndef otbSpectralInformationDivergenceFunctor_h
#define otbSpectralInformationDivergenceFunctor_h

#include "otbMath.h"
#include <algorithm>
#include <vector>

namespace otb
{
namespace Functor
{

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
    
    TInput inputProbability = ComputeProbabilityMassFunction(input);
    
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
    m_ReferenceProbabilities.clear();
    for (auto const & pixel : ref)
    {
      m_ReferenceProbabilities.push_back(ComputeProbabilityMassFunction(pixel));
    }
  }
  
private:
  inline TInput ComputeProbabilityMassFunction(TInput const & input) const
  {
    double sum = 0.0;
    for (unsigned int i = 0; i < input.Size(); i++)
    {
      // Input pixel should be non negative (e.g. reflectance, radiance)
      if (input[i] <= 0) 
      {
        throw std::domain_error("Input pixel of the spectral information divergence algorithm should be strictly positive.");
      }
      sum += input[i];
    }
    TInput res(input.Size());
    for (unsigned int i = 0; i < input.Size(); i++)
    {
      res[i] = input[i] / sum;
    }
    return res;
  }

  inline OutputValueType ComputeSpectralInformationDivergence(TInput const & p, TInput const & q) const
  {
    assert(p.Size() == q.Size());
    OutputValueType sid = 0.0;
    for (unsigned int i = 0; i < p.Size(); i++)
    {
      sid += p[i] * std::log(p[i]/q[i]) + q[i] * std::log(q[i]/p[i]);
    }
    return sid;
  }

  /** Probability mass function associated with the */ 
  std::vector<TReference> m_ReferenceProbabilities;
};

} // end namespace functor
} // end namespace otb

#endif //otbSpectralInformationDivergenceFunctor_h
