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

#ifndef __otbScalarToRainbowRGBPixelFunctor_h
#define __otbScalarToRainbowRGBPixelFunctor_h

#include "itkRGBPixel.h"
#include "itkScalarToRGBPixelFunctor.h"

namespace otb {

namespace Functor {  

/**
 * \class ScalarToRainbowRGBPixelFunctor
 * \brief Function object which maps a scalar value into a rainbow RGB pixel value.
 *
 *  This class is useful for visualizing images which cannot be mapped
 *  succefully into grayscale images.  
 * 
 *  Grayscale image is mapped to a color image where blue represents 
 * small values and red represents big values.
 *
 */
template< class TScalar >
class ITK_EXPORT ScalarToRainbowRGBPixelFunctor : 
    public itk::Functor::ScalarToRGBPixelFunctor<TScalar>
{
public:
  ScalarToRainbowRGBPixelFunctor();
  ~ScalarToRainbowRGBPixelFunctor() {};

  typedef unsigned char               RGBComponentType;
  typedef itk::RGBPixel<RGBComponentType>  RGBPixelType;
  typedef TScalar                     ScalarType;
  
  RGBPixelType operator()( const TScalar &) const;
  
  /** Set the input maximum to be mapped to red */
  void SetMaximum(ScalarType max)
  {
    this->m_Maximum = max;
  }
  
  /** Set the input minimum to be mapped to blue */
  void SetMinimum(ScalarType min)
  {
    this->m_Minimum = min;
  }

  protected:
  RGBPixelType HSVToRGB(double h, double s, double v) const;
  
private:
  ScalarType m_Maximum;
  ScalarType m_Minimum;
};
  
} // end namespace functor

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbScalarToRainbowRGBPixelFunctor.txx"
#endif

#endif
