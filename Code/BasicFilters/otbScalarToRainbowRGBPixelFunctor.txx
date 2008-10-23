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

namespace otb {

namespace Functor {  

template <class TScalar>
ScalarToRainbowRGBPixelFunctor<TScalar>
::ScalarToRainbowRGBPixelFunctor()
{
  m_Minimum = 0;
  m_Maximum = itk::NumericTraits<ScalarType>::max();
  
}

  
template <class TScalar>
typename ScalarToRainbowRGBPixelFunctor<TScalar>::RGBPixelType
ScalarToRainbowRGBPixelFunctor<TScalar>
::operator()( const TScalar & v) const
{
  
  double hinc, sinc, vinc;
  hinc=0.6/(m_Maximum-m_Minimum);
  sinc=0.0;
  vinc=0.0;
  
  double hue, sat, val;

  hue = 0.6 - (v-m_Minimum)*hinc;
  if (v < m_Minimum)
  {
    hue=0.6;
  }
  if (v > m_Maximum)
  {
    hue =0.0;
  }  
  sat = 0.99 + v*sinc;
  val = itk::NumericTraits<RGBComponentType>::max() + v*vinc;
  
  return HSVToRGB(hue, sat, val);
  
}


//----------------------------------------------------------------------------
template <class TScalar>
typename ScalarToRainbowRGBPixelFunctor<TScalar>::RGBPixelType
ScalarToRainbowRGBPixelFunctor<TScalar>
  ::HSVToRGB(double h, double s, double v) const
{
  double onethird = 1.0 / 3.0;
  double onesixth = 1.0 / 6.0;
  double twothird = 2.0 / 3.0;
  double fivesixth = 5.0 / 6.0;
  double r, g, b;
  
  // compute RGB from HSV
  if (h > onesixth && h <= onethird) // green/red
    {
    g = 1.0;
    r = (onethird - h) / onesixth;
    b = 0.0;
    }
  else if (h > onethird && h <= 0.5) // green/blue
    {
    g = 1.0;
    b = (h - onethird) / onesixth;
    r = 0.0;
    }
  else if (h > 0.5 && h <= twothird) // blue/green
    {
    b = 1.0;
    g = (twothird - h) / onesixth;
    r = 0.0;
    }
  else if (h > twothird && h <= fivesixth) // blue/red
    {
    b = 1.0;
    r = (h - twothird) / onesixth;
    g = 0.0;
    }
  else if (h > fivesixth && h <= 1.0) // red/blue
    {
    r = 1.0;
    b = (1.0 - h) / onesixth;
    g = 0.0;
    }
  else // red/green
    {
    r = 1.0;
    g = h / onesixth;
    b = 0.0;
    }
  
  // add Saturation to the equation.
  r = (s * r + (1.0 - s));
  g = (s * g + (1.0 - s));
  b = (s * b + (1.0 - s));
  
  r *= v;
  g *= v;
  b *= v;
  
  RGBPixelType ans;
  ans[0] = static_cast<RGBComponentType>( r );
  ans[1] = static_cast<RGBComponentType>( g );
  ans[2] = static_cast<RGBComponentType>( b );
  return ans;
  
}



} // end namespace Functor

} // end namespace otb


#endif
