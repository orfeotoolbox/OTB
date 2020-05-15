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

#ifndef otbRadiometricMomentsFunctor_h
#define otbRadiometricMomentsFunctor_h

#include "itkVariableLengthVector.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class RadiometricMomentsFunctor
 *  \brief
 *
 *  \ingroup Functor
 *
 * \ingroup OTBMoments
 */
template <class TNeighIter, class TPrecision = float>
class RadiometricMomentsFunctor
{
public:
  typedef RadiometricMomentsFunctor                 Self;
  typedef TNeighIter                                IteratorType;
  typedef typename IteratorType::PixelType          PixelType;
  typedef TPrecision                                ScalarRealType;
  typedef itk::VariableLengthVector<ScalarRealType> OutputType;

  RadiometricMomentsFunctor()
  {
  }
  ~RadiometricMomentsFunctor()
  {
  }

  inline OutputType operator()(TNeighIter& it) const
  {
    OutputType moments;
    moments.SetSize(4);
    moments.Fill(itk::NumericTraits<ScalarRealType>::Zero);

    ScalarRealType sum1, sum2, sum3, sum4;
    sum1 = itk::NumericTraits<ScalarRealType>::Zero;
    sum2 = itk::NumericTraits<ScalarRealType>::Zero;
    sum3 = itk::NumericTraits<ScalarRealType>::Zero;
    sum4 = itk::NumericTraits<ScalarRealType>::Zero;

    // it.GoToBegin();
    const unsigned int size = it.Size();
    for (unsigned int i = 0; i < size; ++i)
    {
      ScalarRealType value  = static_cast<ScalarRealType>(it.GetPixel(i));
      ScalarRealType value2 = value * value;

      // Update cumulants
      sum1 += value;
      sum2 += value2;
      sum3 += value * value2;
      sum4 += value2 * value2;
    }

    // final computations
    // Mean
    moments[0] = sum1 / size;
    // Variance
    moments[1] = (sum2 - (sum1 * moments[0])) / (size - 1);

    ScalarRealType sigma = std::sqrt(moments[1]);
    ScalarRealType mean2 = moments[0] * moments[0];

    const double epsilon = 1E-10;
    if (std::abs(moments[1]) > epsilon)
    {
      // Skewness
      moments[2] = ((sum3 - 3.0 * moments[0] * sum2) / size + 2.0 * moments[0] * mean2) / (moments[1] * sigma);
      // Kurtosis
      moments[3] = ((sum4 - 4.0 * moments[0] * sum3 + 6.0 * mean2 * sum2) / size - 3.0 * mean2 * mean2) / (moments[1] * moments[1]) - 3.0;
    }

    return moments;
  }
};
}
}

#endif
