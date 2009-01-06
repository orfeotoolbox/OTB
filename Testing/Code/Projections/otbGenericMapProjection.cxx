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

#include "otbMacro.h"
#include "otbImage.h"
#include "otbGenericMapProjection.h"

//TODO add control baseline
int otbGenericMapProjection( int argc, char* argv[] )
{
  const char * outFileName = argv[1];
  std::ofstream file;
  file.open(outFileName);

  /** Test the ability to instanciate a projection from a string*/
  std::string projectionRefWkt ="PROJCS[\"UTM Zone 31, Northern Hemisphere\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],PARAMETER[\"central_meridian\",3],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],PARAMETER[\"false_northing\",0],UNIT[\"Meter\",1]]";

  typedef otb::GenericMapProjection<otb::FORWARD> GenericMapProjection;
  GenericMapProjection::Pointer genericMapProjection = GenericMapProjection::New();
  genericMapProjection->SetWkt(projectionRefWkt);
  file << genericMapProjection->GetWkt() << std::endl << std::endl;

  file.close();
  return EXIT_SUCCESS;

}