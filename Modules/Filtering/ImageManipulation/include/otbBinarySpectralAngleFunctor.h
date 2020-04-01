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
  inline TOutputValue operator()(const TInput1& a, const TInput2& b) const
  {
    return SpectralAngleDetails::ComputeSpectralAngle<TInput1, TInput2, TOutputValue>(a, b, b.GetNorm());
  }
};

} // end namespace Functor
} // end namespace otb

#endif
