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

#ifndef otbSampleAugmentation_h
#define otbSampleAugmentation_h

#ifdef _OPENMP
#include <omp.h>
#endif

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cassert>

namespace otb
{

namespace sampleAugmentation
{
using SampleType       = std::vector<double>;
using SampleVectorType = std::vector<SampleType>;

/**
Estimate standard deviations of the components in one pass using
Welford's algorithm
*/
SampleType EstimateStds(const SampleVectorType& samples)
{
  const auto nbSamples    = samples.size();
  const long nbComponents = static_cast<long>(samples[0].size());
  SampleType stds(nbComponents, 0.0);
  SampleType means(nbComponents, 0.0);
  for (size_t i = 0; i < nbSamples; ++i)
  {
    auto norm_factor = 1.0 / (i + 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (long j = 0; j < nbComponents; ++j)
    {
      const auto mu    = means[j];
      const auto x     = samples[i][j];
      auto       muNew = mu + (x - mu) * norm_factor;
      stds[j] += (x - mu) * (x - muNew);
      means[j] = muNew;
    }
  }
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (long j = 0; j < nbComponents; ++j)
  {
    stds[j] = std::sqrt(stds[j] / nbSamples);
  }
  return stds;
}

/** Create new samples by replicating input samples. We loop through
* the input samples and add them to the new data set until nbSamples
* are added. The elements of newSamples are removed before proceeding.
*/
void ReplicateSamples(const SampleVectorType& inSamples, const size_t nbSamples, SampleVectorType& newSamples)
{
  newSamples.resize(nbSamples);
  const long long nbSamplesLL = static_cast<long long>(nbSamples);
  size_t          imod{0};
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (long long i = 0; i < nbSamplesLL; ++i)
  {
    if (imod == inSamples.size())
      imod        = 0;
    newSamples[i] = inSamples[imod++];
  }
}

/** Create new samples by adding noise to existing samples. Gaussian
* noise is added to randomly selected samples. The standard deviation
* of the noise added to each component is the same as the one of the
* input variables divided by stdFactor (defaults to 10). The
* elements of newSamples are removed before proceeding.
*/
void JitterSamples(const SampleVectorType& inSamples, const size_t nbSamples, SampleVectorType& newSamples, float stdFactor = 10,
                   const int seed = std::time(nullptr))
{
  newSamples.resize(nbSamples);
  const long         nbComponents = static_cast<long>(inSamples[0].size());
  std::random_device rd;
  std::mt19937       gen(rd());
  // The input samples are selected randomly with replacement
  std::srand(seed);
  // We use one gaussian distribution per component since they may
  // have different stds
  auto                                         stds = EstimateStds(inSamples);
  std::vector<std::normal_distribution<double>> gaussDis(nbComponents);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (long i   = 0; i < nbComponents; ++i)
    gaussDis[i] = std::normal_distribution<double>{0.0, stds[i] / stdFactor};

  for (size_t i = 0; i < nbSamples; ++i)
  {
    newSamples[i] = inSamples[std::rand() % inSamples.size()];
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (long j = 0; j < nbComponents; ++j)
      newSamples[i][j] += gaussDis[j](gen);
  }
}


struct NeighborType
{
  size_t index;
  double distance;
};

struct NeighborSorter
{
  constexpr bool operator()(const NeighborType& a, const NeighborType& b) const
  {
    return b.distance > a.distance;
  }
};

double ComputeSquareDistance(const SampleType& x, const SampleType& y)
{
  assert(x.size() == y.size());
  double dist{0};
  for (size_t i = 0; i < x.size(); ++i)
  {
    dist += (x[i] - y[i]) * (x[i] - y[i]);
  }
  return dist / (x.size() * x.size());
}

using NNIndicesType = std::vector<NeighborType>;
using NNVectorType  = std::vector<NNIndicesType>;
/** Returns the indices of the nearest neighbors for each input sample
*/
void FindKNNIndices(const SampleVectorType& inSamples, const size_t nbNeighbors, NNVectorType& nnVector)
{
  const long long nbSamples = static_cast<long long>(inSamples.size());
  nnVector.resize(nbSamples);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (long long sampleIdx = 0; sampleIdx < nbSamples; ++sampleIdx)
  {
    NNIndicesType nns;
    for (long long neighborIdx = 0; neighborIdx < nbSamples; ++neighborIdx)
    {
      if (sampleIdx != neighborIdx)
        nns.push_back({static_cast<size_t>(neighborIdx), ComputeSquareDistance(inSamples[sampleIdx], inSamples[neighborIdx])});
    }
    std::partial_sort(nns.begin(), nns.begin() + nbNeighbors, nns.end(), NeighborSorter{});
    nns.resize(nbNeighbors);
    nnVector[sampleIdx] = std::move(nns);
  }
}

/** Generate the new sample in the line linking s1 and s2
*/
SampleType SmoteCombine(const SampleType& s1, const SampleType& s2, double position)
{
  auto result = s1;
  for (size_t i = 0; i < s1.size(); ++i)
    result[i]   = s1[i] + (s2[i] - s1[i]) * position;
  return result;
}

/** Create new samples using the SMOTE algorithm
Chawla, N. V., Bowyer, K. W., Hall, L. O., & Kegelmeyer, W. P., Smote:
synthetic minority over-sampling technique, Journal of artificial
intelligence research, 16(), 321–357 (2002).
http://dx.doi.org/10.1613/jair.953
*/
void Smote(const SampleVectorType& inSamples, const size_t nbSamples, SampleVectorType& newSamples, const int nbNeighbors, const int seed = std::time(nullptr))
{
  newSamples.resize(nbSamples);
  const long long nbSamplesLL = static_cast<long long>(nbSamples);
  NNVectorType    nnVector;
  FindKNNIndices(inSamples, nbNeighbors, nnVector);
  // The input samples are selected randomly with replacement
  std::srand(seed);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (long long i = 0; i < nbSamplesLL; ++i)
  {
    const auto sampleIdx   = std::rand() % (inSamples.size());
    const auto sample      = inSamples[sampleIdx];
    const auto neighborIdx = nnVector[sampleIdx][std::rand() % nbNeighbors].index;
    const auto neighbor    = inSamples[neighborIdx];
    newSamples[i]          = SmoteCombine(sample, neighbor, std::rand() / double{RAND_MAX});
  }
}

} // end namespaces sampleAugmentation
} // end namespace otb

#endif
