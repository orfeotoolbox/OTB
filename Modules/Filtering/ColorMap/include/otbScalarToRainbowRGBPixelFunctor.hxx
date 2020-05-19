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


#ifndef otbScalarToRainbowRGBPixelFunctor_hxx
#define otbScalarToRainbowRGBPixelFunctor_hxx

#include "otbScalarToRainbowRGBPixelFunctor.h"

namespace otb
{

namespace Functor
{

template <class TScalar, class TRGBPixel>
ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>::ScalarToRainbowRGBPixelFunctor()
{
  //  m_Minimum = 0;
  //  m_Maximum = itk::NumericTraits<ScalarType>::max();
}

template <class TScalar, class TRGBPixel>
typename ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>::RGBPixelType ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>::operator()(const TScalar& v) const
{

  double hinc, sinc, vinc;
  hinc = 0.6 / (this->GetMaximumInputValue() - this->GetMinimumInputValue());
  sinc = 0.0;
  vinc = 0.0;

  double hue, sat, val;

  hue = 0.6 - (v - this->GetMinimumInputValue()) * hinc;
  if (v < this->GetMinimumInputValue())
  {
    hue = 0.6;
  }
  if (v > this->GetMaximumInputValue())
  {
    hue = 0.0;
  }
  sat = 0.99 + v * sinc;
  val = itk::NumericTraits<RGBComponentType>::max() + v * vinc;

  return m_HSVToRGBFunctor(hue, sat, val);
}

} // end namespace Functor
} // end namespace otb

#endif
