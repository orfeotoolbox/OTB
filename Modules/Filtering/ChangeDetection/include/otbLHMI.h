/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbLHMI_h
#define otbLHMI_h

#include <vector>
#include "itkHistogram.h"

namespace otb
{

namespace Functor
{

/** \class LHMI
 * \brief TODO
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the difference of the two pixel values
 * - compute the value of the LHMI
 * - cast the \c double value resulting to the pixel type of the output image
 *
 *
 * \ingroup OTBChangeDetection
 */

template<class TInput1, class TInput2, class TOutput>
class LHMI
{

public:

  typedef typename std::vector<TOutput>                                  VectorType;
  typedef typename VectorType::iterator                                  IteratorType;
  typedef typename std::vector<VectorType>                               VectorOfVectorType;
  typedef typename VectorOfVectorType::iterator                          VecOfVecIteratorType;
  typedef double                                                         HistogramFrequencyType;
  typedef typename itk::Statistics::Histogram<HistogramFrequencyType,
                             itk::Statistics::DenseFrequencyContainer2 > HistogramType;
  typedef typename HistogramType::MeasurementVectorType
  MeasurementVectorType;
  typedef typename HistogramType::SizeType HistogramSizeType;
  typedef typename HistogramType::Iterator HistogramIteratorType;

  LHMI() {}
  virtual ~LHMI() {}
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB)
  {
    HistogramType::Pointer histogram;

    /** The histogram size. */
    HistogramSizeType histogramSize(2);
    /** The lower bound for samples in the histogram. */
    MeasurementVectorType lowerBound(2);
    /** The upper bound for samples in the histogram. */
    MeasurementVectorType upperBound(2);

    double upperBoundIncreaseFactor = 0.001;

    histogramSize.Fill(256);

    TOutput maxA = itA.GetPixel(0);
    TOutput minA = itA.GetPixel(0);
    TOutput maxB = itB.GetPixel(0);
    TOutput minB = itB.GetPixel(0);

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {

      TOutput value = static_cast<TOutput>(itA.GetPixel(pos));

      if (value > maxA) maxA = value;
      else if (value < minA) minA = value;

      value = static_cast<TOutput>(itB.GetPixel(pos));

      if (value > maxB) maxB = value;
      else if (value < minB) minB = value;

      }

    // Initialize the upper and lower bounds of the histogram.
    lowerBound[0] = minA;
    lowerBound[1] = minB;
    upperBound[0] =
      maxA + (maxA - minA) * upperBoundIncreaseFactor;
    upperBound[1] =
      maxB + (maxB - minB) * upperBoundIncreaseFactor;

    histogram = HistogramType::New();

    histogram->SetMeasurementVectorSize(2);
    histogram->Initialize(histogramSize, lowerBound, upperBound);

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {

      typename HistogramType::IndexType sample(2);
      sample[0] = itA.GetPixel(pos);
      sample[1] = itB.GetPixel(pos);
      /*if(sample[0]!=NumericTraits<TOutput>::Zero &&
         sample[1]!=NumericTraits<TOutput>::Zero)*/
      histogram->IncreaseFrequencyOfIndex(sample, 1);

      }

    TOutput                entropyX = itk::NumericTraits<TOutput>::Zero;
    TOutput                entropyY = itk::NumericTraits<TOutput>::Zero;
    TOutput                jointEntropy = itk::NumericTraits<TOutput>::Zero;
    HistogramFrequencyType totalFreq = histogram->GetTotalFrequency();

    for (unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
      {
      HistogramFrequencyType freq = histogram->GetFrequency(i, 0);
      if (freq > 0)
        {
        entropyX += freq * std::log(freq);
        }
      }

    entropyX = -entropyX / static_cast<TOutput>(totalFreq) + std::log(totalFreq);

    for (unsigned int i = 0; i < histogram->GetSize()[1]; ++i)
      {
      HistogramFrequencyType freq = histogram->GetFrequency(i, 1);
      if (freq > 0)
        {
        entropyY += freq * std::log(freq);
        }
      }

    entropyY = -entropyY / static_cast<TOutput>(totalFreq) + std::log(totalFreq);

    HistogramIteratorType it = histogram->Begin();
    HistogramIteratorType end = histogram->End();
    while (it != end)
      {
      HistogramFrequencyType freq = it.GetFrequency();
      if (freq > 0)
        {
        jointEntropy += freq * std::log(freq);
        }
      ++it;
      }

    jointEntropy = -jointEntropy / static_cast<TOutput>(totalFreq) +
                   std::log(totalFreq);

    return static_cast<TOutput>(jointEntropy / (entropyX + entropyY));
  }

};
}

}
#endif
