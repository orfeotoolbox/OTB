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
#ifndef __otbReliefColormapFunctor_txx
#define __otbReliefColormapFunctor_txx

#include "otbReliefColormapFunctor.h"

namespace otb {

namespace Functor {

template <class TScalar, class TRGBPixel>
typename ReliefColormapFunctor<TScalar, TRGBPixel>::RGBPixelType
ReliefColormapFunctor<TScalar, TRGBPixel>
::operator() (const TScalar &v) const
{

  float m_Borders[]     = {0.0,  0.00001, 0.2,  0.43, 0.71, 1.0};
  float m_RedValues[]   = {0.75, 0.64,    0.47, 0.89, 0.58, 1.0};
  float m_GreenValues[] = {0.91, 0.84,    0.76, 0.90, 0.32, 1.0};
  float m_BlueValues[]  = {0.96, 0.59,    0.40, 0.57, 0.00, 1.0};

  // Map the input scalar between [0, 1].
  RealType value = this->RescaleInputValue(v);

  int i = 1;
  while (value > m_Borders[i])
    ++i;

  float factor = (value - m_Borders[i - 1]) / (m_Borders[i] - m_Borders[i - 1]);

  // Apply the color mapping.
  RealType red = m_RedValues[i - 1] + factor * (m_RedValues[i] - m_RedValues[i - 1]);

  RealType green = m_GreenValues[i - 1] + factor * (m_GreenValues[i] - m_GreenValues[i - 1]);

  RealType blue = m_BlueValues[i - 1] + factor * (m_BlueValues[i] - m_BlueValues[i - 1]);

  // Normalize the values
  red = vnl_math_max(0.0, red);
  red = vnl_math_min(1.0, red);

  green = vnl_math_max(0.0, green);
  green = vnl_math_min(1.0, green);

  blue = vnl_math_max(0.0, blue);
  blue = vnl_math_min(1.0, blue);

  // Set the rgb components after rescaling the values.
  RGBPixelType pixel;

  pixel[0] = this->RescaleRGBComponentValue(red);
  pixel[1] = this->RescaleRGBComponentValue(green);
  pixel[2] = this->RescaleRGBComponentValue(blue);

  return pixel;
}

} // end namespace Functor

} // end namespace otb

#endif
