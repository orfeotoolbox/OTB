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


#include <fstream>
#include <iomanip>

#include "otbGenericMapProjection.h"
#include "ogr_spatialref.h"
int otbGenericMapProjection(int itkNotUsed(argc), char* argv[])
{
  const char*   outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  file << std::setprecision(15);

  /** Test the ability to instantiate a projection from a string*/
  std::string projectionRefWkt =
      "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, "
      "AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], "
      "UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], "
      "PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], "
      "PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";
  auto baselineSpatialReference = otb::SpatialReference::FromDescription(projectionRefWkt);

  typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> GenericMapProjection;
  GenericMapProjection::Pointer                                       genericMapProjection = GenericMapProjection::New();
  genericMapProjection->SetWkt(projectionRefWkt);

  auto testSpatialReferenceForward = otb::SpatialReference::FromDescription(genericMapProjection->GetWkt());

  if (testSpatialReferenceForward != baselineSpatialReference)
  {
    std::cerr << "The spatial reference used in the forward test is different from the input spatial reference" << std::endl;
    return EXIT_FAILURE;
  }

  itk::Point<double, 2> point;
  point[0] = 1.44;
  point[1] = 43.6050;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjection->TransformPoint(point);
  file << std::endl << std::endl;

  typedef otb::GenericMapProjection<otb::TransformDirection::INVERSE> GenericMapProjectionInverse;
  GenericMapProjectionInverse::Pointer                                genericMapProjectionInverse = GenericMapProjectionInverse::New();
  genericMapProjectionInverse->SetWkt(projectionRefWkt);

  auto testSpatialReferenceReverse = otb::SpatialReference::FromDescription(genericMapProjectionInverse->GetWkt());

  if (testSpatialReferenceForward != baselineSpatialReference)
  {
    std::cerr << "The spatial reference used in the reverse test is different from the input spatial reference" << std::endl;
    return EXIT_FAILURE;
  }

  point[0] = 374100.8;
  point[1] = 4829184.8;
  file << "Inverse projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjectionInverse->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
