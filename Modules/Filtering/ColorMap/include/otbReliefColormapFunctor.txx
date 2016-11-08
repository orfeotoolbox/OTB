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
#ifndef otbReliefColormapFunctor_txx
#define otbReliefColormapFunctor_txx

#include "otbReliefColormapFunctor.h"

namespace otb {

namespace Functor {

template <class TScalar, class TRGBPixel>
typename ReliefColormapFunctor<TScalar, TRGBPixel>::RGBPixelType
ReliefColormapFunctor<TScalar, TRGBPixel>
::operator() (const TScalar &v) const
{

  float m_Borders[]     = {0.0f,  0.00001f, 0.2f,  0.43f, 0.71f, 1.0f};
  float m_RedValues[]   = {0.75f, 0.64f,    0.47f, 0.89f, 0.58f, 1.0f};
  float m_GreenValues[] = {0.91f, 0.84f,    0.76f, 0.90f, 0.32f, 1.0f};
  float m_BlueValues[]  = {0.96f, 0.59f,    0.40f, 0.57f, 0.00f, 1.0f};

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
