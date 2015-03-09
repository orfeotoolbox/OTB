/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbScalarToRainbowRGBPixelFunctor_txx
#define __otbScalarToRainbowRGBPixelFunctor_txx

#include "otbScalarToRainbowRGBPixelFunctor.h"

namespace otb
{

namespace Functor
{

template <class TScalar, class TRGBPixel>
ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>
::ScalarToRainbowRGBPixelFunctor()
{
//  m_Minimum = 0;
//  m_Maximum = itk::NumericTraits<ScalarType>::max();

}

template <class TScalar, class TRGBPixel>
typename ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>::RGBPixelType
ScalarToRainbowRGBPixelFunctor<TScalar, TRGBPixel>
::operator() (const TScalar &v) const
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
