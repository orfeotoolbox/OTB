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


#include "otbEllipsoidAdapter.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/base/ossimEllipsoid.h"
#pragma GCC diagnostic pop
#else
#include "ossim/base/ossimEllipsoid.h"
#endif


namespace otb
{

EllipsoidAdapter::EllipsoidAdapter()
{
  m_Ellipsoid = new ossimEllipsoid();
}

EllipsoidAdapter::~EllipsoidAdapter()
{
  if (m_Ellipsoid != nullptr)
  {
    delete m_Ellipsoid;
  }
}

void EllipsoidAdapter::XYZToLonLatHeight(double x, double y, double z, double& lon, double& lat, double& h) const
{
  // Note the lat/lon convension for ossim vs lon/lat for OTB
  m_Ellipsoid->XYZToLatLonHeight(x, y, z, lat, lon, h);
}

void EllipsoidAdapter::LonLatHeightToXYZ(double lon, double lat, double h, double& x, double& y, double& z) const
{
  // Note the lat/lon convension for ossim vs lon/lat for OTB
  m_Ellipsoid->latLonHeightToXYZ(lat, lon, h, x, y, z);
}

} // namespace otb
