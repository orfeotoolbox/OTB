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

#ifndef otbGeocentricTransform_hxx
#define otbGeocentricTransform_hxx

#include "otbGeocentricTransform.h"
#include "otbMath.h"

namespace otb
{

template <TransformDirection TDirectionOfMapping, class TScalarType>
GeocentricTransform<TDirectionOfMapping, TScalarType>::GeocentricTransform() : Superclass(ParametersDimension)
{
  SpatialReference wgs84               = SpatialReference::FromWGS84();
  SpatialReference ecefSpatialReference = SpatialReference::FromDescription("EPSG:4978");

#if GDAL_VERSION_NUM >= 3000000
  wgs84.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  ecefSpatialReference.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif

  if (DirectionOfMapping == TransformDirection::INVERSE)
  {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(ecefSpatialReference, wgs84));

    if (newMapProjection)
      m_MapProjection.swap(newMapProjection);
  }
  else
  {
    std::unique_ptr<CoordinateTransformation> newMapProjection(new CoordinateTransformation(wgs84, ecefSpatialReference));

    if (newMapProjection)
      m_MapProjection.swap(newMapProjection);
  }

  m_a = 6378137.;
  m_b = 6356752.314245;
  m_f = (m_a - m_b)/m_a;
  const double e = std::sqrt(1 - (m_b * m_b) / (m_a * m_a));
  m_es = e * e;
}

template <TransformDirection TDirectionOfMapping, class TScalarType>
typename GeocentricTransform<TDirectionOfMapping, TScalarType>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType>::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  // Geographic to geocentric
  if (DirectionOfMapping == TransformDirection::FORWARD)
  {
    const auto lon = point[0]* CONST_PI_180;
    const auto lat = point[1]* CONST_PI_180;
    const auto height = point[2];

    auto sin_latitude = std::sin(lat);
    auto cos_latitude = std::cos(lat);
    auto N = m_a / sqrt( 1.0 - m_es * sin_latitude * sin_latitude);
    outputPoint[0] = (N + height) * cos_latitude * cos(lon);
    outputPoint[1] = (N + height) * cos_latitude * sin(lon);
    outputPoint[2] = (N * (1 - m_es) + height) * sin_latitude;
  }
  // Geocentric to geographic
  else
  {
    const auto & x = point[0];
    const auto & y = point[1];
    const auto & z = point[2];

    const double tol = 1e-15;
    const double d = sqrt(x*x + y*y);
    const int MAX_ITER = 10;

    const double a2 = m_a * m_a;
    const double b2 = m_b * m_b;
    const double pa2 = d * d * a2;
    const double qb2 = z * z * b2;
    const double ae2 = a2 * m_es;
    const double ae4 = ae2 * ae2;

    const double c3 = -( ae4/2 + pa2 + qb2 );          // s^2
    const double c4 = ae2*( pa2 - qb2 );               // s^1
    const double c5 = ae4/4 * ( ae4/4 - pa2 - qb2 );   // s^0

    double s0 = 0.5 * (a2 + b2) * std::hypot( d/m_a, z/m_b );

    for( int iterIdx = 0; iterIdx < MAX_ITER; ++iterIdx )
    {
      const double pol = c5 + s0 * ( c4 + s0 * ( c3 + s0 * s0 ) );
      const double der = c4 + s0 * ( 2 * c3  + 4 * s0 * s0 );

      const double ds = - pol / der;
      s0 += ds;

      if( fabs( ds ) < tol * s0 )
      {
        const double t = s0 - 0.5 * (a2 + b2);
        const double x_ell = d / ( 1.0 + t/a2 );
        const double y_ell = z / ( 1.0 + t/b2 );

        double height = ( d - x_ell ) * x_ell/a2 + ( z - y_ell ) * y_ell/b2;
        height /= std::hypot( x_ell/a2 ,  y_ell/b2 );

        //lat
        outputPoint[1] = std::atan2( y_ell/b2, x_ell/a2 ) * CONST_180_PI;

        //lon
        outputPoint[0] = std::atan2( y, x ) * CONST_180_PI;
        outputPoint[2] = height;
      }
    }
  }
 // std::cout << "Transformation : " << point << " -> " << outputPoint << std::endl;
  //OutputPointType outputPoint2;
  //std::tie(outputPoint2[0], outputPoint2[1], outputPoint2[2]) = m_MapProjection->Transform(std::make_tuple(point[0], point[1], point[2]));
  //std::cout << "With GDAL : " << point << " -> " << outputPoint2 << std::endl;
  return outputPoint;
}

/*
template <TransformDirection TDirectionOfMapping, class TScalarType>
typename GeocentricTransform<TDirectionOfMapping, TScalarType>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType>::TransformPoint(const InputPointType& point) const
{
  OutputPointType outputPoint;

  std::tie(outputPoint[0], outputPoint[1], outputPoint[2]) = m_MapProjection->Transform(std::make_tuple(point[0], point[1], point[2]));

  return outputPoint;
}

*/

} // namespace otb
#endif
