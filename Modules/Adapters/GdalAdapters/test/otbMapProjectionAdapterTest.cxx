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


#include <fstream>
#include <iomanip>

#include "otbMapProjectionAdapter.h"

int otbMapProjectionAdapterTest(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  file << std::setprecision(15);

  {
  /** Test the ability to instantiate a projection from a wkt string*/
  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instantiating from : " << projectionRefWkt << std::endl;
  file << genericMapProjection->GetWkt() << std::endl << std::endl;

  double lon = 1.44;
  double lat = 43.6050;
  double h = 10;
  double x = 0;
  double y = 0;
  double z = 0;
  genericMapProjection->ForwardTransform(lon, lat, h, x, y, z);
  file << "Forward projection: " << std::endl;
  file << lon << ", " << lat << ", " << h << " -> ";
  file << x << ", " << y << ", " << z;
  file << std::endl << std::endl;

  genericMapProjection->InverseTransform(x, y, z, lon, lat, h);
  file << "Inverse projection: " << std::endl;
  file << x << ", " << y << ", " << z << " -> ";
  file << lon << ", " << lat << ", " << h;
  file << std::endl << std::endl;
  }
  
  file.close();
  return EXIT_SUCCESS;
}
