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

#include "otbImageMetadata.h"

std::ostream& operator<<(std::ostream& os, const otb::ImageMetadata& imd)
{
  os << "ImageMetadata [";
  os << "SensorID:'" << imd.SensorID << "';";
  os << "ProjectionRef:'"<<imd.ProjectionRef<<"';";
  os << "GCPProjection:'"<<imd.GCPProjection<<"';";
  os << "GCPs[";
  for (auto &gcp : imd.GCPs)
    {
    gcp.Print(os);
    }
  os << "];";
  os << "GeoTransform: ["
      << imd.GeoTransform[0] << ","
      << imd.GeoTransform[1] << ","
      << imd.GeoTransform[2] << ","
      << imd.GeoTransform[3] << ","
      << imd.GeoTransform[4] << ","
      << imd.GeoTransform[5] << "]";
  os << "Bands[";
  for (auto &band : imd.Bands)
    {
    os << band << ",";
    }
  os << "]";
  // TODO : finish printing other fields
  return os;
}
