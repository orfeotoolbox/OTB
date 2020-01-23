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

#ifndef otbImageMetadata_h
#define otbImageMetadata_h

#include "otbBandMetadata.h"
//#include "otbGeometryMetadata.h"
#include "otbMetaDataKey.h"
#include "OTBMetadataExport.h"

#include <boost/any.hpp>
#include <vector>
#include <string>
#include <ctime>

namespace otb
{

/** \class ImageMetadata
 *
 * \brief Generic class containing image metadatas used in OTB
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT ImageMetadata
{
// ------------------------------ static part ----------------------------------

  std::string ProjectionRef;

  std::string GCPProjection;

  std::vector<OTB_GCP> GCPs;

  double GeoTransform[6] = {0.0, 1.0, 0.0, 0.0, 0.0, 1.0};

  std::string SensorID;

  double PixelSizeX = 0.0;
  double PixelSizeY = 0.0;

  /** Acquisition date */
  struct tm AcquisitionDate;

  /** Production date */
  struct tm ProductionDate;
  
  /** Corners coordinate */
  double ULX = 0.0;
  double ULY = 0.0;
  double URX = 0.0;
  double URY = 0.0;
  double LLX = 0.0;
  double LLY = 0.0;
  double LRX = 0.0;
  double LRY = 0.0;
  
  /** Band-specific metadatas */
  std::vector<BandMetadata> Bands;

  /** Sensor model geometry */
  boost::any SensorGeometry;

// ------------------------------ dynamic part ---------------------------------

  std::map<MetaData::NumericKeyType, double> NumericKeys;

  std::map<MetaData::StringKeyType, std::string> StringKeys;

  std::map<MetaData::LUT1DKeyType, MetaData::LUT1D > LUT1DKeys;

  std::map<MetaData::LUT2DKeyType, MetaData::LUT2D > LUT2DKeys;
  
};

} // end namespace otb

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

#endif
