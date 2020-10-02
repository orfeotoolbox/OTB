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


#include "otbRemoteSensingRegion.h"

int otbRemoteSensingRegion(int itkNotUsed(argc), char* argv[])
{

  typedef double Type;

  const Type startX = atof(argv[1]);
  const Type startY = atof(argv[2]);
  const Type sizeX  = atof(argv[3]);
  const Type sizeY  = atof(argv[4]);

  const Type pointX = atof(argv[5]);
  const Type pointY = atof(argv[6]);

  typedef otb::RemoteSensingRegion<Type> TypedRegion;

  TypedRegion region;

  /** */
  TypedRegion::SizeType  size;
  TypedRegion::IndexType index, indexPt;

  size[0] = sizeX;
  size[1] = sizeY;

  index[0] = startX;
  index[1] = startY;

  region.SetSize(size);
  region.SetOrigin(index);

  /** Check if the segment is inside the region*/
  indexPt[0] = pointX;
  indexPt[1] = pointY;

  if (region.IsInside(indexPt))
    std::cout << "Le point " << indexPt[0] << "," << indexPt[1] << " is inside the region " << std::endl;
  else
    std::cout << "Le point  is OUTSIDE the region " << std::endl;

  /*Set the projection */
  std::string projectionRefWkt =
      "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, "
      "AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], "
      "UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], "
      "PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], "
      "PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  region.SetRegionProjection(projectionRefWkt);

  std::cout << "The projection set is " << region.GetRegionProjection() << std::endl;

  return EXIT_SUCCESS;
}
