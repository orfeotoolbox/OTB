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

#ifndef otbHillShadingFunctor_h
#define otbHillShadingFunctor_h

#include "itkNumericTraits.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class HillShadeModulationFunctor
 *  \brief Modulate an image with hill shading
 *
 *  Take an image (RGB) as input and the result of the hillshading
 *  (another image with the value between 0 and 1) and modulate
 *  the first image by the value of the hill shading.
 *
 *  \ingroup Functor
 *  \example BasicFilters/HillShadingExample.cxx
 *
 * \ingroup OTBImageManipulation
*/
template<class TInput1, class TInput2 = TInput1, class TOutput = TInput1>
class HillShadeModulationFunctor
{
public:
  HillShadeModulationFunctor() {}
  ~HillShadeModulationFunctor() {}

  inline TOutput operator ()(const TInput1& A, const TInput2& B) const
  {
    TOutput out;
    out.SetRed( static_cast<typename TOutput::ValueType>(static_cast<double>(A.GetRed()) * static_cast<double>(B)) );
    out.SetGreen( static_cast<typename TOutput::ValueType>(static_cast<double>(A.GetGreen()) * static_cast<double>(B)) );
    out.SetBlue(static_cast<typename TOutput::ValueType>(static_cast<double>(A.GetBlue()) * static_cast<double>(B)));
    return out;
  }
};

/** \class HillShadingFunctor
 *  \brief Unary neighborhood functor to compute the lambertian of a surface
 *
 *  The light source is assumed to be at a given elevation and azimuth
 *  (by default \f$ \pi/4 \f$ and \f$ \pi/6 \f$  respectively). This is used to generate hill shading
 *  representation of relief. The output is a value between 0 and 1.
 *
 *  \ingroup Functor
 *  \example BasicFilters/HillShadingExample.cxx
 *
 * \ingroup OTBImageManipulation
*/
template <class TNeighIter, class TInputImage, class TOutput>
class HillShadingFunctor
{
public:

  typedef HillShadingFunctor               Self;
  typedef TNeighIter                       IteratorType;
  typedef typename IteratorType::PixelType PixelType;

  HillShadingFunctor() : m_AzimuthLight(30.0 * CONST_PI_180), m_ElevationLight(45.0 *CONST_PI_180),
    m_XRes(100.0), m_YRes(100.0), m_Scale(0.1)
  {
    m_SinElev = std::sin(m_ElevationLight);
    m_CosElev = std::cos(m_ElevationLight);
    m_SinAz = std::sin(m_AzimuthLight);
    m_CosAz = std::cos(m_AzimuthLight);
  }
  ~HillShadingFunctor() {}

  double GetXRes() const
  {
    return m_XRes;
  }

  double GetYRes() const
  {
    return m_YRes;
  }

  void SetXRes(double res)
  {
    m_XRes = std::abs(res);
  }

  void SetYRes(double res)
  {
    m_YRes = std::abs(res);
  }

  double GetScale() const
  {
    return m_Scale;
  }

  void SetScale(double scale)
  {
    m_Scale = scale;
  }

  double GetAzimuthLight() const
  {
    return m_AzimuthLight;
  }

  void SetAzimuthLight(double az)
  {
    m_AzimuthLight = az;
    m_SinAz = std::sin(m_AzimuthLight);
    m_CosAz = std::cos(m_AzimuthLight);
  }

  double GetElevationLight() const
  {
    return m_ElevationLight;
  }

  void SetElevationLight(double el)
  {
    m_ElevationLight = el;
    m_SinElev = std::sin(m_ElevationLight);
    m_CosElev = std::cos(m_ElevationLight);
  }

  inline TOutput operator ()(const TNeighIter& it) const
  {
    const typename IteratorType::OffsetType LEFT   = {{-1, 0}};
    const typename IteratorType::OffsetType RIGHT  = {{1, 0}};
    const typename IteratorType::OffsetType UP     = {{0, -1}};
    const typename IteratorType::OffsetType DOWN   = {{0, 1}};
    const typename IteratorType::OffsetType LEFTUP   = {{-1, -1}};
    const typename IteratorType::OffsetType RIGHTDOWN = {{1, 1}};
    const typename IteratorType::OffsetType RIGHTUP  = {{1, -1}};
    const typename IteratorType::OffsetType LEFTDOWN = {{-1, 1}};
//    const typename IteratorType::OffsetType CENTER ={{0, 0}};

    float xSlope =
      ((makeValid(it.GetPixel(LEFTUP)) + 2 * makeValid(it.GetPixel(LEFT)) + makeValid(it.GetPixel(LEFTDOWN)))
       - (makeValid(it.GetPixel(RIGHTUP)) + 2 * makeValid(it.GetPixel(RIGHT)) + makeValid(it.GetPixel(RIGHTDOWN))))
      / (m_XRes * m_Scale);
    // - as the azimuth is given compared to y axis pointing up
    float ySlope = -((makeValid(it.GetPixel(LEFTUP)) + 2 * makeValid(it.GetPixel(UP)) + makeValid(it.GetPixel(RIGHTUP)))
                     - (makeValid(it.GetPixel(LEFTDOWN)) + 2 * makeValid(it.GetPixel(DOWN)) + makeValid(it.GetPixel(RIGHTDOWN)))
                     )
                   / (m_YRes * m_Scale);

    // permutation between x and y as the azimuth angle is given compared to the north-south axis
    float lambertian = ((m_CosElev * m_CosAz * ySlope) + (m_CosElev * m_SinAz * xSlope) + m_SinElev)
                       / std::sqrt(xSlope * xSlope + ySlope * ySlope + 1);

    return (lambertian + 1) / 2; //normalize between 0 and 1

  }

private:

  inline PixelType makeValid(PixelType v) const
  {
    return v < itk::NumericTraits<PixelType>::Zero ? itk::NumericTraits<PixelType>::Zero : v;
  }

  double m_AzimuthLight; // in radian
  double m_ElevationLight; // in radian
  double m_XRes; // assumed to be positive provided in m
  double m_YRes; // assumed to be positive provided in m
  double m_Scale;

  // precomputed parameters to avoid the sin() cos() call for each pixel
  double m_SinElev;
  double m_CosElev;
  double m_SinAz;
  double m_CosAz;

};

}
}

#endif
