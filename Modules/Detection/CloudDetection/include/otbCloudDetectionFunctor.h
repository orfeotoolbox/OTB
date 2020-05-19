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

#ifndef otbCloudDetectionFunctor_h
#define otbCloudDetectionFunctor_h

#include "otbCloudEstimatorFunctor.h"

namespace otb
{
/** \class CloudDetectionFunctor
 *  \brief This functor first uses CloudEstimatorFunctor
 *  \brief And then binarise the image with two thresholds
 *
 * \ingroup OTBCloudDetection
 */
namespace Functor
{
template <class TInput, class TOutputValue>
class CloudDetectionFunctor
{
public:
  typedef CloudEstimatorFunctor<TInput, TOutputValue> CloudEstimatorFunctorType;

  CloudDetectionFunctor()
  {
    m_MinThreshold = 0.0;
    m_MaxThreshold = 1.0;
  }

  virtual ~CloudDetectionFunctor()
  {
  }
  inline TOutputValue operator()(const TInput& inPix)
  {
    if ((m_CloudEstimatorFunctor(inPix) > m_MinThreshold) && (m_CloudEstimatorFunctor(inPix) <= m_MaxThreshold))
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  void SetReferencePixel(TInput ref)
  {
    m_CloudEstimatorFunctor.SetReferencePixel(ref);
  }
  void SetVariance(double variance)
  {
    m_CloudEstimatorFunctor.SetVariance(variance);
  }
  void SetMinThreshold(double threshold)
  {
    m_MinThreshold = threshold;
  }
  void SetMaxThreshold(double threshold)
  {
    m_MaxThreshold = threshold;
  }
  double GetMinThreshold()
  {
    return m_MinThreshold;
  }
  double GetMaxThreshold()
  {
    return m_MaxThreshold;
  }
  double GetVariance()
  {
    return m_CloudEstimatorFunctor.GetVariance();
  }
  TInput GetReferencePixel()
  {
    return m_CloudEstimatorFunctor.GetReferencePixel();
  }

protected:
  CloudEstimatorFunctorType m_CloudEstimatorFunctor;
  double                    m_MinThreshold;
  double                    m_MaxThreshold;
};

} // end namespace functor
} // end namespace otb

#endif
