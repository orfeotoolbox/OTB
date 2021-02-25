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

#ifndef otbSharkUtils_h
#define otbSharkUtils_h

#include <stdexcept>
#include <string>
#include <unordered_map>

// Quiet a deprecation warning
#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wheader-guard"

#if defined(__apple_build_version__)
/* Need AppleClang >= 9.0.0 to support -Wexpansion-to-defined */
#if __apple_build_version__ >= 9000000
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#endif
#elif __clang_major__ > 3
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#endif

#endif
#endif
#include "otb_shark.h"
#include "shark/Data/Dataset.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{
namespace Shark
{
template <class T>
void ListSampleRangeToSharkVector(const T* listSample, std::vector<shark::RealVector>& output, unsigned int start, unsigned int size)
{
  assert(listSample != nullptr);

  if (start + size > listSample->Size())
  {
    std::out_of_range e_(
        std::string("otb::Shark::ListSampleRangeToSharkVector "
                    ": Requested range is out of list sample bounds"));
    throw e_;
  }

  output.clear();

  // Sample index
  unsigned int sampleIdx = start;

  // Check for valid listSample
  if (listSample->Size() > 0)
  {
    // Retrieve samples size alike
    const unsigned int sampleSize = listSample->GetMeasurementVectorSize();

    // Fill the output vector

    for (auto const endOfRange = start + size; sampleIdx < endOfRange; ++sampleIdx)
    {
      // Retrieve sample
      typename T::MeasurementVectorType const& sample = listSample->GetMeasurementVector(sampleIdx);

      // // Define a shark::RealVector
      // shark::RealVector rv(sampleSize);
      // // Loop on sample size
      // for(unsigned int i = 0; i < sampleSize; ++i)
      //   {
      //   rv[i] = sample[i];
      //   }
      // using std::move;
      // output.emplace_back(move(rv));

      output.emplace_back(&sample[0], &sample[0] + sampleSize);
    }
  }
}

template <class T>
void ListSampleRangeToSharkVector(const T* listSample, std::vector<unsigned int>& output, unsigned int start, unsigned int size)
{
  assert(listSample != nullptr);

  if (start + size > listSample->Size())
  {
    std::out_of_range e_(
        std::string("otb::Shark::ListSampleRangeToSharkVector "
                    ": Requested range is out of list sample bounds"));
    throw e_;
  }

  output.clear();

  // Sample index
  unsigned int sampleIdx = start;

  // Check for valid listSample
  if (listSample->Size() > 0)
  {
    // Fill the output vector
    while (sampleIdx < start + size)
    {
      // Retrieve sample
      typename T::MeasurementVectorType const& sample = listSample->GetMeasurementVector(sampleIdx);

      // Define a shark::RealVector
      output.push_back(sample[0]);
      ++sampleIdx;
    }
  }
}

template <class T>
void ListSampleToSharkVector(const T* listSample, std::vector<shark::RealVector>& output)
{
  assert(listSample != nullptr);
  ListSampleRangeToSharkVector(listSample, output, 0U, static_cast<unsigned int>(listSample->Size()));
}

template <class T>
void ListSampleToSharkVector(const T* listSample, std::vector<unsigned int>& output)
{
  assert(listSample != nullptr);
  ListSampleRangeToSharkVector(listSample, output, 0, static_cast<unsigned int>(listSample->Size()));
}

/** Shark assumes that labels are 0 ... (nbClasses-1). This function modifies the labels contained in the input vector and returns a vector with size =
nbClasses which allows the translation from the normalised labels to the new ones oldLabel = dictionary[newLabel]. When we want to generate the image containing
the probability for each class, we need to ensure that the probabilities are in the correct order wrt the incoming labels. We therefore sort the labels before
building the encoding.
*/
template <typename T>
void NormalizeLabelsAndGetDictionary(std::vector<T>& labels, std::vector<T>& dictionary)
{
  std::vector<T> sorted_labels = labels;
  std::sort(std::begin(sorted_labels), std::end(sorted_labels));
  auto last = std::unique(std::begin(sorted_labels), std::end(sorted_labels));
  sorted_labels.erase(last, std::end(sorted_labels));
  std::unordered_map<T, T> dictMap;
  T                        labelCount{0};
  for (const auto& l : sorted_labels)
  {
    if (dictMap.find(l) == dictMap.end())
      dictMap.insert({l, labelCount++});
  }
  dictionary.resize(labelCount);
  for (auto& l : labels)
  {
    auto newLabel        = dictMap[l];
    dictionary[newLabel] = l;
    l                    = newLabel;
  }
}

} // namespace Shark
} // namespace otb

#endif
