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


#ifndef otbScalarToRainbowRGBPixelFunctor_h
#define otbScalarToRainbowRGBPixelFunctor_h

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
 *
 * \ingroup OTBColorMap
 */
template <class TRGBPixel>
class ITK_EXPORT HSVToRGBFunctor
{
public:
  HSVToRGBFunctor(){};
  ~HSVToRGBFunctor()
  {
  }
  inline TRGBPixel operator()(double h, double s, double v) const
  {
    const double onethird  = 1.0 / 3.0;
    const double onesixth  = 1.0 / 6.0;
    const double twothird  = 2.0 / 3.0;
    const double fivesixth = 5.0 / 6.0;
    double       r, g, b;

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

    //         std::cout << h << ", " << s << ", " << v << " -> " << r << ", " << g << ", " << b << std::endl;

    TRGBPixel                                 ans;
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
 *
 * \ingroup OTBColorMap
 */
template <class TScalar, class TRGBPixel = itk::RGBPixel<unsigned char>>
class ITK_EXPORT ScalarToRainbowRGBPixelFunctor : public itk::Function::ColormapFunction<TScalar, TRGBPixel>
//      public itk::Functor::ScalarToRGBPixelFunctor<TScalar>
{
public:
  ScalarToRainbowRGBPixelFunctor();
  ~ScalarToRainbowRGBPixelFunctor() override
  {
  }

  typedef ScalarToRainbowRGBPixelFunctor Self;
  typedef itk::Function::ColormapFunction<TScalar, TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TRGBPixel                            RGBPixelType;
  typedef typename RGBPixelType::ComponentType RGBComponentType;
  typedef TScalar                              ScalarType;
  typedef HSVToRGBFunctor<RGBPixelType>        HSVToRGBFunctorType;

  RGBPixelType operator()(const TScalar&) const override;

protected:
  RGBPixelType HSVToRGB(double h, double s, double v) const;

private:
  ScalarToRainbowRGBPixelFunctor(const Self&) = delete;
  void operator=(const Self&) = delete;

  HSVToRGBFunctorType m_HSVToRGBFunctor;
};

} // end namespace functor

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarToRainbowRGBPixelFunctor.hxx"
#endif

#endif
