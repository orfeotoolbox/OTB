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

#ifndef otbSqrtSpectralAngleFunctor_h
#define otbSqrtSpectralAngleFunctor_h

#include "otbSpectralAngleFunctor.h"
#include "itkMacro.h"

namespace otb
{

namespace Functor
{

/** \class SqrtSpectralAngleFunctor
 *  \brief This functor computes the square root of a spectral angle according to a reference pixel.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputVectorPixel, class TOutputPixel>
class SqrtSpectralAngleFunctor : public SpectralAngleFunctor<TInputVectorPixel, TOutputPixel>
{
public:
  typedef SqrtSpectralAngleFunctor Self;
  typedef SpectralAngleFunctor<TInputVectorPixel, TOutputPixel> Superclass;

  SqrtSpectralAngleFunctor()
  {
  }
  ~SqrtSpectralAngleFunctor() override
  {
  }

protected:
  TOutputPixel Evaluate(const TInputVectorPixel& inPix) const override
  {
    return static_cast<TOutputPixel>(std::sqrt(Superclass::Evaluate(inPix)));
  }
};

} // end namespace Functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
