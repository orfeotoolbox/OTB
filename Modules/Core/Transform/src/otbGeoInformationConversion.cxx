/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbGeoInformationConversion.h"

#include "ogr_spatialref.h"
#include "ogr_core.h"

#include "cpl_conv.h"
#include "boost/lexical_cast.hpp"
#include "itkMacro.h"

namespace otb
{

std::string GeoInformationConversion::ToWKT(int srid)
{
  char * wkt = nullptr;
  std::string ret;

  // Build the srs from the epsg number
  OGRSpatialReferenceH hSRS = nullptr;
  hSRS = OSRNewSpatialReference(nullptr);

  if (OSRImportFromEPSG(hSRS, srid ) == OGRERR_NONE)
    {
    // Get the wkt
    if ( OSRExportToWkt(hSRS, &wkt) == OGRERR_NONE)
      {
      ret = wkt;
      CPLFree(wkt);
      }
    }
  OSRRelease(hSRS);
  return  ret;
}

std::string GeoInformationConversion::ToWKT(const std::string& candidateSrid)
{
  std::string wkt(candidateSrid);
  try
    {
    int srid = boost::lexical_cast<int>(candidateSrid);
    return GeoInformationConversion::ToWKT(srid);
    }
  catch(boost::bad_lexical_cast &)
    {
    return wkt;
    }
}

bool GeoInformationConversion::IsESRIValidWKT(const std::string &wkt)
{
  OGRSpatialReference SRS(wkt.c_str());
  SRS.morphToESRI();
  return SRS.Validate()==OGRERR_NONE;
}

} // End namespace otb
