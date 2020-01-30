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
  //~ struct tm AcquisitionDate;

  /** Production date */
  //~ struct tm ProductionDate;
  
  /** Corners coordinate */
  double ULX = 0.0;
  double ULY = 0.0;
  double URX = 0.0;
  double URY = 0.0;
  double LLX = 0.0;
  double LLY = 0.0;
  double LRX = 0.0;
  double LRY = 0.0;

  /** Tile Hint */
  unsigned int TileHintX = 0;
  unsigned int TileHintY = 0;

  int DataType = 0;
  
  /** Band-specific metadatas */
  std::vector<BandMetadata> Bands;

  // TODO: check the concept, maybe use boost/type_erasure with properties ostreamable, ...
  /** Sensor model geometry */
  boost::any SensorGeometry;

// ------------------------------ dynamic part ---------------------------------

  std::map<MDNum, double> NumericKeys;

  std::map<MDStr, std::string> StringKeys;

  std::map<MDL1D, MetaData::LUT1D > LUT1DKeys;

  std::map<MDL2D, MetaData::LUT2D > LUT2DKeys;

  std::map<MDTime, MetaData::Time> TimeKeys;
};



extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::ImageMetadata& imd);

} // end namespace otb

#endif
