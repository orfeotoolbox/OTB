/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMeanRatio_h
#define otbMeanRatio_h

#include "otbBinaryFunctorNeighborhoodImageFilter.h"

namespace otb
{

/** \class Functor::MeanRatio
 *
 * - compute the ratio of the two pixel values
 * - compute the value of the ratio of means
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *

 * \ingroup Functor
 *
 * \ingroup OTBChangeDetection
 */
namespace Functor
{

template <class TInput1, class TInput2, class TOutput>
class MeanRatio
{
public:
  MeanRatio()
  {
  }
  virtual ~MeanRatio()
  {
  }
  inline TOutput operator()(const TInput1& itA, const TInput2& itB)
  {

    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
    {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));
    }

    meanA /= itA.Size();
    meanB /= itB.Size();

    // std::cout<<"meanA= "<<meanA<<", meanB= "<<meanB<<std::endl;

    TOutput ratio;

    if (meanA == meanB)
      ratio = 0.;
    else if (meanA > meanB)
      ratio = static_cast<TOutput>(1.0 - meanB / meanA);
    else
      ratio = static_cast<TOutput>(1.0 - meanA / meanB);

    return ratio;
  }
};
}
} // end namespace otb

#endif
