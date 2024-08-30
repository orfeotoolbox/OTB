/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
typename GeocentricTransform<TDirectionOfMapping, TScalarType>::OutputPointType
GeocentricTransform<TDirectionOfMapping, TScalarType>::TransformPoint(const InputPointType& point) const
{
  // Geographic to geocentric
  if (DirectionOfMapping == TransformDirection::FORWARD)
  {
    return Projection::WorldToEcef(point);
  }
  // Geocentric to geographic
  else
  {
    return Projection::EcefToWorld(point);
  }
}

namespace Projection
{

template <class TScalarType, class TEllipsoid>
itk::Point<TScalarType, 3> WorldToEcef(const itk::Point<TScalarType, 3> & worldPoint)
{
  itk::Point<TScalarType, 3> ecefPoint;

  const auto lon = worldPoint[0]* CONST_PI_180;
  const auto lat = worldPoint[1]* CONST_PI_180;
  const auto height = worldPoint[2];

  auto sin_latitude = std::sin(lat);
  auto cos_latitude = std::cos(lat);
  auto N = TEllipsoid::a / sqrt( 1.0 - TEllipsoid::es * sin_latitude * sin_latitude);

  ecefPoint[0] = (N + height) * cos_latitude * cos(lon);
  ecefPoint[1] = (N + height) * cos_latitude * sin(lon);
  ecefPoint[2] = (N * (1 - TEllipsoid::es) + height) * sin_latitude;

  return ecefPoint;
}

template <class TScalarType, class TEllipsoid>
itk::Point<TScalarType, 3> EcefToWorld(const itk::Point<TScalarType, 3> & ecefPoint)
{
  itk::Point<TScalarType, 3> worldPoint;

  const auto x = ecefPoint[0];
  const auto y = ecefPoint[1];
  const auto z = ecefPoint[2];

  const TScalarType tol = 1e-15;
  const TScalarType d2 = x*x + y*y;
  const TScalarType d = sqrt(d2);
  const int MAX_ITER = 10;

  constexpr TScalarType a2 = TEllipsoid::a * TEllipsoid::a;
  constexpr TScalarType b2 = TEllipsoid::b * TEllipsoid::b;
  const TScalarType pa2 = d2 * a2;
  const TScalarType qb2 = z * z * b2;
  constexpr TScalarType ae2 = a2 * TEllipsoid::es;
  constexpr TScalarType ae4 = ae2 * ae2;

  const TScalarType c3 = -( ae4/2 + pa2 + qb2 );          // s^2
  const TScalarType c4 = ae2*( pa2 - qb2 );               // s^1
  const TScalarType c5 = ae4/4 * ( ae4/4 - pa2 - qb2 );   // s^0

  TScalarType s0 = 0.5 * (a2 + b2) * std::sqrt(d/TEllipsoid::a * d/TEllipsoid::a + z/TEllipsoid::b * z/TEllipsoid::b);

  for (int iterIdx = 0; iterIdx < MAX_ITER; ++iterIdx)
  {
    const TScalarType pol = c5 + s0 * ( c4 + s0 * ( c3 + s0 * s0 ) );
    const TScalarType der = c4 + s0 * ( 2 * c3  + 4 * s0 * s0 );

    const TScalarType ds = - pol / der;
    s0 += ds;

    if (std::abs(ds) < tol * s0)
    {
      constexpr TScalarType half = 0.5;
      constexpr TScalarType one = 1.0;

      const auto t = s0 - half * (a2 + b2);
      const auto x_ell = d / (one + t/a2);
      const auto y_ell = z / (one + t/b2);

      auto const xea2 = x_ell / a2;
      auto const yeb2 = y_ell / b2;

      auto height = (d - x_ell) * xea2 + (z - y_ell) * yeb2;
      height /= std::sqrt(xea2 * xea2 + yeb2 * yeb2);

      //lat
      worldPoint[1] = std::atan2(yeb2, xea2) * CONST_180_PI;

      //lon
      worldPoint[0] = std::atan2(y, x) * CONST_180_PI;
      worldPoint[2] = height;
    }
  }
  return worldPoint;
}

} // namespace Projection




} // namespace otb
#endif
