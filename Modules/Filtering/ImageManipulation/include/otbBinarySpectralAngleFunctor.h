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

#ifndef otbBinarySpectralAngleFunctor_h
#define otbBinarySpectralAngleFunctor_h

#include <algorithm>
#include "otbMath.h"
#include "otbSpectralAngleFunctor.h"

namespace otb
{
/** \class BinarySpectralAngleFunctor
 *  \brief This functor computes the spectral angle between two pixels.
 *
 *  If the pixels have different sizes, only the first components of the 
 *  largest pixel will be considered.
 *
 *  It can be used as a functor in a BinaryFunctorImageFilter to
 *  compute the pixel-by-pixel spectral angles.
 *
 * \ingroup OTBImageManipulation
 */
namespace Functor
{
template <class TInput1, class TInput2, class TOutputValue>
class BinarySpectralAngleFunctor
{
public:
  BinarySpectralAngleFunctor() = default;

  virtual ~BinarySpectralAngleFunctor() = default;

  // Binary operator
  TOutputValue operator()(const TInput1& in1, const TInput2& in2) const
  {
    // Compute norms.
    auto in1Norm = 0;
    auto in2Norm = 0;
    auto nbIter = std::min(in1.Size(), in2.Size());
    for (unsigned int i = 0; i < nbIter; ++i)
    {
      in1Norm += in1[i] * in1[i];
      in2Norm += in2[i] * in2[i];
    }
    
    return SpectralAngleDetails::ComputeSpectralAngle<TInput1, TInput2, TOutputValue>(in1, in1Norm, in2, in2Norm);
  }
};

} // end namespace Functor
} // end namespace otb

#endif
