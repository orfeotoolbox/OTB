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

#include "otbMapProjectionAdapter.h"

int otbMapProjectionAdapterTest(int itkNotUsed(argc), char* argv[])
{
  const char *  outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  file << std::setprecision(15);

  {
  /** Test the ability to instanciate a projection from a wkt string*/
  std::string projectionRefWkt =
    "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instanciating from : " << projectionRefWkt << std::endl;
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

  {
  /** Test the ability to instanciate a projection from an ossim string*/
  std::string projectionRefWkt ="ossimUtmProjection";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instanciating from : " << projectionRefWkt << std::endl;
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

  {
  /** Test the ability to instanciate a projection from an short string*/
  std::string projectionRefWkt ="Utm";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instanciating from : " << projectionRefWkt << std::endl;
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

  {
  /** Test the ability to instanciate a projection from an short string*/
  std::string projectionRefWkt ="Mercator";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instanciating from : " << projectionRefWkt << std::endl;
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

  {
  /** Test the ability to instanciate a projection from an short string*/
  std::string projectionRefWkt ="LambertConformalConic";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << "Instanciating from : " << projectionRefWkt << std::endl;
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


  {
  /** Test the ability to instanciate an Utm projection in a different zone */
  std::string projectionRefWkt ="Utm";

  otb::MapProjectionAdapter::Pointer genericMapProjection = otb::MapProjectionAdapter::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  genericMapProjection->SetParameter("Zone", "46");
  genericMapProjection->SetParameter("Hemisphere", "S");
  file << "Instanciating from : " << projectionRefWkt << " with some extra parameters" << std::endl;
  file << genericMapProjection->GetWkt() << std::endl << std::endl;

  double lon = 91.44;
  double lat = -43.6050;
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
