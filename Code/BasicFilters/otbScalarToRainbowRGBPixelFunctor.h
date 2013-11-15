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
#include "itkColormapFunction.h"

namespace otb
{

namespace Functor
{

/**
 * \class HSVToRGBFunctor
 * \brief Function object to convert HSV value to RGB.
 *
 *
 * \sa ScalarToRainbowRGBPixelFunctor
 *
 */
template<class TRGBPixel>
class ITK_EXPORT HSVToRGBFunctor
{
public:
  HSVToRGBFunctor(){};
  ~HSVToRGBFunctor(){}
  inline TRGBPixel operator ()(double h, double s, double v) const
  {
    const double onethird = 1.0 / 3.0;
    const double onesixth = 1.0 / 6.0;
    const double twothird = 2.0 / 3.0;
    const double fivesixth = 5.0 / 6.0;
    double       r, g, b;

    // compute RGB from HSV
    if (h > onesixth && h <= onethird)     // green/red
      {
      g = 1.0;
      r = (onethird - h) / onesixth;
      b = 0.0;
      }
    else if (h > onethird && h <= 0.5)     // green/blue
      {
      g = 1.0;
      b = (h - onethird) / onesixth;
      r = 0.0;
      }
    else if (h > 0.5 && h <= twothird)     // blue/green
      {
      b = 1.0;
      g = (twothird - h) / onesixth;
      r = 0.0;
      }
    else if (h > twothird && h <= fivesixth)     // blue/red
      {
      b = 1.0;
      r = (h - twothird) / onesixth;
      g = 0.0;
      }
    else if (h > fivesixth && h <= 1.0)     // red/blue
      {
      r = 1.0;
      b = (1.0 - h) / onesixth;
      g = 0.0;
      }
    else     // red/green
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

//         std::cout << h << ", " << s << ", " << v << " -> " << r << ", " << g << ", " << b << std::endl;

    TRGBPixel ans;
    typedef typename TRGBPixel::ComponentType RGBComponentType;
    ans[0] = static_cast<RGBComponentType>(r);
    ans[1] = static_cast<RGBComponentType>(g);
    ans[2] = static_cast<RGBComponentType>(b);
    return ans;

  }
};
}

namespace Functor
{

/**
 * \class ScalarToRainbowRGBPixelFunctor
 * \brief Function object which maps a scalar value into a rainbow RGB pixel value.
 *
 *  Grayscale image is mapped to a color image where blue represents
 * small values and red represents big values.
 *
 * This function is compatible with the colormap hierarchy presented in
 * http://hdl.handle.net/1926/1452
 *
 * \example BasicFilters/DEMToRainbowExample.cxx
 *
 */
template<class TScalar, class TRGBPixel = itk::RGBPixel<unsigned char> >
class ITK_EXPORT ScalarToRainbowRGBPixelFunctor
  : public itk::Function::ColormapFunction<TScalar, TRGBPixel>
//      public itk::Functor::ScalarToRGBPixelFunctor<TScalar>
{
public:
  ScalarToRainbowRGBPixelFunctor();
  ~ScalarToRainbowRGBPixelFunctor() {}

  typedef ScalarToRainbowRGBPixelFunctor                      Self;
  typedef itk::Function::ColormapFunction<TScalar, TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TRGBPixel                            RGBPixelType;
  typedef typename RGBPixelType::ComponentType RGBComponentType;
  typedef TScalar                              ScalarType;
  typedef HSVToRGBFunctor<RGBPixelType>        HSVToRGBFunctorType;

  RGBPixelType operator ()(const TScalar&) const;

  /** Set the input maximum to be mapped to red
   * \deprecated use SetMaximumInputValue() */
  void SetMaximum(ScalarType max)
  {
    this->SetMaximumInputValue(max);
  }

  /** Set the input minimum to be mapped to blue
   * \deprecated use SetMinimumInputValue() */
  void SetMinimum(ScalarType min)
  {
    this->SetMinimumInputValue(min);
  }

protected:
  RGBPixelType HSVToRGB(double h, double s, double v) const;

private:
  ScalarToRainbowRGBPixelFunctor(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  HSVToRGBFunctorType m_HSVToRGBFunctor;
};

} // end namespace functor

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarToRainbowRGBPixelFunctor.txx"
#endif

#endif
