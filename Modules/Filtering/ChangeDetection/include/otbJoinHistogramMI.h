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

#ifndef otbJoinHistogramMI_h
#define otbJoinHistogramMI_h

#include "itkHistogram.h"

namespace otb
{

namespace Functor
{

template<class TInput1, class TInput2, class TOutput>
class JoinHistogramMI
{
public:
  typedef double                                                         HistogramFrequencyType;
  typedef typename itk::Statistics::Histogram<HistogramFrequencyType,
                             itk::Statistics::DenseFrequencyContainer2 > HistogramType;
  JoinHistogramMI() {}
  virtual ~JoinHistogramMI() {}
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB, const HistogramType* histogram)
  {
    TOutput                jointEntropy = itk::NumericTraits<TOutput>::Zero;
    HistogramFrequencyType totalFreq = histogram->GetTotalFrequency();

    typename HistogramType::MeasurementVectorType sample(2);
    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {
      double valueA = static_cast<double>(itA.GetPixel(pos));
      double valueB = static_cast<double>(itB.GetPixel(pos));

      sample[0] = valueA;
      sample[1] = valueB;

      typename HistogramType::IndexType index;
      histogram->GetIndex(sample, index);
      HistogramFrequencyType freq = histogram->GetFrequency(index);
      if (freq > 0)
        {
        jointEntropy += freq * std::log(freq);
        }

      }

    jointEntropy = -jointEntropy / static_cast<TOutput>(totalFreq) +
                   std::log(totalFreq);

    return jointEntropy;
  }

};
}
} // end namespace otb

#endif
