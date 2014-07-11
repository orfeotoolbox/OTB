/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <fstream>
#include <iomanip>

#include "otbGenericMapProjection.h"

int otbGenericMapProjection(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  file << std::setprecision(15);

  /** Test the ability to instanciate a projection from a string*/
  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> GenericMapProjection;
  GenericMapProjection::Pointer genericMapProjection = GenericMapProjection::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << genericMapProjection->GetWkt() << std::endl << std::endl;

  itk::Point<double, 2> point;
  point[0] = 1.44;
  point[1] = 43.6050;
  file << "Forward projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjection->TransformPoint(point);
  file << std::endl << std::endl;

  typedef otb::GenericMapProjection<otb::TransformDirection::INVERSE> GenericMapProjectionInverse;
  GenericMapProjectionInverse::Pointer genericMapProjectionInverse = GenericMapProjectionInverse::New();
  genericMapProjectionInverse->SetWkt(projectionRefWkt);
  file << genericMapProjectionInverse->GetWkt() << std::endl << std::endl;

  point[0] = 374100.8;
  point[1] = 4829184.8;
  file << "Inverse projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjectionInverse->TransformPoint(point);
  file << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;

}
